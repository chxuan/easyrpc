#include "listen_address.h"
#include "easyrpc/utility/logger.h"
#include "easyrpc/core/codec/server_codec.h"
#include "easyrpc/core/net/io_service_pool.h"
#include "easyrpc/core/net/tcp_session.h"

listen_address::listen_address(std::shared_ptr<io_service_pool>& pool) 
    : pool_(pool), 
    acceptor_(pool_->get_io_service())
{

}

bool listen_address::listen(const std::string& ip, unsigned short port)
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
        log_error() << e.what();
        return false;
    }

    return true;
}

void listen_address::accept()
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
