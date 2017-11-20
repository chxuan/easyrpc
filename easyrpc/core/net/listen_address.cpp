#include "listen_address.h"
#include "easyrpc/utility/logger.h"
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
#if 0
        auto new_conn = std::make_shared<connection>(io_service_pool::singleton::get()->get_io_service(), 
                                                     route_, handle_error_, client_connect_notify_, client_disconnect_notify_);
        acceptor_.async_accept(new_conn->socket(), [this, new_conn](boost::system::error_code ec)
        {
            if (!ec)
            {
                new_conn->start();
            }
            accept();
        });
#endif
}
