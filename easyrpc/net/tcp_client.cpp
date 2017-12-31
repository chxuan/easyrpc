#include "tcp_client.h"
#include "tcp_session.h"
#include "io_service_pool.h"
#include "easyrpc/utility/utiltiy.h"
#include "easyrpc/utility/logger.h"
#include "easyrpc/codec/codec.h"

tcp_client::tcp_client(const std::string& address)
    : address_(address),
    pool_(std::make_shared<io_service_pool>(1))
{

}

tcp_client::~tcp_client()
{
    stop();
}

void tcp_client::set_session_status_callback(const std::function<void(bool, const std::string&)>& func)
{
    session_status_callback_ = func;
}

bool tcp_client::run()
{
    pool_->run();

    if (!parse_connect_address())
    {
        return false;
    }

    session_ = std::make_shared<tcp_session>(codec_, pool_->get_io_service(), 
                                             std::bind(&tcp_client::deal_session_closed, this, std::placeholders::_1));
    if (connect(session_->get_socket()))
    {
        session_->run();
        deal_session_established();
        return true;
    }

    return false;
}

void tcp_client::stop()
{
    session_->close();
    pool_->stop();
}

void tcp_client::async_write(const std::shared_ptr<std::string>& network_data)
{
    session_->async_write(network_data);
}

bool tcp_client::parse_connect_address()
{
    std::string ip;
    unsigned short port = 0;
    if (utiltiy::get_ip_and_port(address_, ip, port))
    {
        boost::asio::ip::tcp::resolver resolver(pool_->get_io_service());
        boost::asio::ip::tcp::resolver::query query(boost::asio::ip::tcp::v4(), ip, std::to_string(port));
        endpoint_iter_ = resolver.resolve(query);
        return true;
    }

    log_warn << "Parse address failed, address: " << address_;
    return false;
}

bool tcp_client::connect(boost::asio::ip::tcp::socket& socket)
{
    try
    {
        boost::asio::connect(socket, endpoint_iter_);
    }
    catch (std::exception& e)
    {
        log_warn << e.what() << ", address: " << address_;
        return false;
    }

    return true;
}

void tcp_client::reconnect()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    boost::asio::async_connect(session_->get_socket(), endpoint_iter_,
                               [this](boost::system::error_code ec, boost::asio::ip::tcp::resolver::iterator)
    {
        if (!ec)
        {
            codec_->reset();
            session_->run();
            deal_session_established();
        }
        else if (ec != boost::asio::error::already_connected)
        {
            reconnect();
        }
    });
}

void tcp_client::deal_session_established()
{
    if (session_status_callback_)
    {
        session_status_callback_(true, session_->get_session_id());
    }
}

void tcp_client::deal_session_closed(const std::string& session_id)
{
    if (session_status_callback_)
    {
        session_status_callback_(false, session_id);
    }

    reconnect();
}
