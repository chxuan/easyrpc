#include "tcp_server.h"
#include "sig.h"
#include "address_listener.h"
#include "tcp_session.h"
#include "io_service_pool.h"
#include "tcp_session_manager.h"
#include "easyrpc/codec/server_codec.h"
#include "easyrpc/utility/logger.h"
#include "easyrpc/utility/utiltiy.h"

tcp_server::tcp_server() 
    : pool_(std::make_shared<io_service_pool>(4)), 
    acceptor_(pool_->get_io_service())
{
    qt_connect(session_status_changed, std::bind(&tcp_server::deal_session_status_changed, 
                                                 this, std::placeholders::_1, std::placeholders::_2));
}

tcp_server::~tcp_server()
{
    stop();
}

tcp_server& tcp_server::listen(const std::string& host)
{
    host_ = host;
    return *this;
}

tcp_server& tcp_server::ios_threads(int num)
{
    ios_threads_ = num;
    return *this;
}

tcp_server& tcp_server::work_threads(int num)
{
    work_threads_ = num;
    return *this;
}

void tcp_server::set_session_status_callback(const std::function<void(bool, const std::string&)>& func)
{
    session_status_callback_ = func;
}

void tcp_server::publish(const std::string& session_id, const std::shared_ptr<google::protobuf::Message>& message)
{
    if (message)
    {
        auto session = manager_->get_session(session_id);
        if (session)
        {
            auto network_data = session->get_codec()->encode(-1, message);
            session->async_write(network_data);
        }
    }
}

bool tcp_server::run()
{
    manager_ = std::make_shared<tcp_session_manager>();
    return start_listen();
}

void tcp_server::stop()
{
    pool_->stop();

    if (manager_)
    {
        manager_->clear();
    }
}

void tcp_server::deal_session_status_changed(bool established, const std::string& session_id)
{
    if (!established)
    {
        manager_->remove_session(session_id);
    }

    if (session_status_callback_)
    {
        session_status_callback_(established, session_id);
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
    std::shared_ptr<codec> codec = std::make_shared<server_codec>();
    auto session = std::make_shared<tcp_session>(codec, pool_->get_io_service());
    acceptor_.async_accept(session->get_socket(), [this, session](boost::system::error_code ec)
    {
        if (!ec)
        {
            session->run();
        }
        accept();
    });
}
