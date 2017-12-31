#include "address_listener.h"
#include "tcp_session.h"
#include "tcp_session_manager.h"
#include "io_service_pool.h"
#include "easyrpc/utility/logger.h"
#include "easyrpc/utility/singletion.h"
#include "easyrpc/utility/utiltiy.h"
#include "easyrpc/codec/server_codec.h"

address_listener::address_listener(const std::string& host, int ios_threads)
    : host_(host),
    pool_(std::make_shared<io_service_pool>(ios_threads)), 
    acceptor_(pool_->get_io_service())
{

}

bool address_listener::start_listen()
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

void address_listener::stop_listen()
{
    pool_->stop();
}

bool address_listener::listen(const std::string& ip, unsigned short port)
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

void address_listener::accept()
{
    std::shared_ptr<codec> codec = std::make_shared<server_codec>();
    auto session = std::make_shared<tcp_session>(codec, pool_->get_io_service());
    acceptor_.async_accept(session->get_socket(), [this, session](boost::system::error_code ec)
    {
        if (!ec)
        {
            session->run();
            singletion<tcp_session_manager>::get_instance().add_session(session->get_session_id(), session);
        }
        accept();
    });
}
