#include "tcp_server.h"
#include "tcp_session.h"
#include "io_service_pool.h"
#include "tcp_session_cache.h"
#include "easyrpc/codec/server_codec.h"
#include "easyrpc/utility/logger.h"
#include "easyrpc/utility/utiltiy.h"

tcp_server::tcp_server(const std::string& host, int ios_threads)
    : host_(host),
    pool_(std::make_shared<io_service_pool>(ios_threads)), 
    acceptor_(pool_->get_io_service())
{

}

tcp_server::~tcp_server()
{
    stop();
}

void tcp_server::set_session_status_callback(const std::function<void(bool, const std::string&)>& func)
{
    session_status_callback_ = func;
}

void tcp_server::publish(const std::string& session_id, const std::shared_ptr<google::protobuf::Message>& message)
{
    if (message)
    {
        auto session = session_cache_->get_session(session_id);
        if (session)
        {
            session->async_write(session->get_codec()->encode(-1, message));
        }
    }
}

bool tcp_server::run()
{
    session_cache_ = std::make_shared<tcp_session_cache>();
    return start_listen();
}

void tcp_server::stop()
{
    pool_->stop();

    if (session_cache_)
    {
        session_cache_->clear();
    }
}

bool tcp_server::start_listen()
{
    pool_->run();

    std::string ip;
    unsigned short port = 0;
    if (!utiltiy::get_ip_and_port(host_, ip, port))
    {
        log_error << "Parse address failed, host: " << host_;
        return false;
    }

    if (listen(ip, port))
    {
        accept();
        return true;
    }

    return false;
}

bool tcp_server::listen(const std::string& ip, unsigned short port)
{
    try
    {
        boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address_v4::from_string(ip), port);
        acceptor_.open(ep.protocol());
        acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
        acceptor_.bind(ep);
        acceptor_.listen();
    }
    catch(std::exception& e)
    {
        log_error << e.what();
        return false;
    }

    return true;
}

void tcp_server::accept()
{
    std::shared_ptr<codec> codec = std::make_shared<server_codec>(std::bind(&tcp_server::deal_client_request, 
                                                                            this, std::placeholders::_1, std::placeholders::_2));
    auto session = std::make_shared<tcp_session>(codec, pool_->get_io_service(), 
                                                 std::bind(&tcp_server::deal_session_closed, this, std::placeholders::_1));
    acceptor_.async_accept(session->get_socket(), [this, session](boost::system::error_code ec)
    {
        if (!ec)
        {
            session->run();
            deal_session_established(session);
        }
        accept();
    });
}

void tcp_server::deal_client_request(const std::shared_ptr<request>& req, const std::shared_ptr<response>& rsp)
{
    deal_request(req, rsp);
}

void tcp_server::deal_session_established(const std::shared_ptr<tcp_session>& session)
{
    session_cache_->add_session(session->get_session_id(), session);
    if (session_status_callback_)
    {
        session_status_callback_(true, session->get_session_id());
    }
}

void tcp_server::deal_session_closed(const std::string& session_id)
{
    session_cache_->remove_session(session_id);
    if (session_status_callback_)
    {
        session_status_callback_(false, session_id);
    }
}

