#include "listen_address.h"
#include "easyrpc/utility/logger.h"
#include "easyrpc/core/codec/server_codec.h"
#include "easyrpc/core/net/io_service_pool.h"
#include "easyrpc/core/net/tcp_session.h"
#include "easyrpc/core/net/listen_address_manager.h"

listen_address::listen_address(std::shared_ptr<io_service_pool>& pool, listen_address_manager* manager) 
    : pool_(pool), 
    acceptor_(pool_->get_io_service()),
    address_manager_(manager)
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
    /* codec_->set_decode_data_callback(std::bind(&rpc_client::decode_data_callback, this, std::placeholders::_1)); */
    auto session = std::make_shared<tcp_session>(codec, pool_->get_io_service());
    session->set_session_status_callback(std::bind(&listen_address_manager::session_status_callback, address_manager_, 
                                                   std::placeholders::_1, std::placeholders::_2));
    acceptor_.async_accept(session->get_socket(), [this, session](boost::system::error_code ec)
    {
        if (!ec)
        {
            session->run();
        }
        accept();
    });
}
