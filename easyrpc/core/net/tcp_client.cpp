#include "tcp_client.h"
#include "crpc/utility/utiltiy.h"
#include "crpc/utility/logger.h"
#include "crpc/core/codec/codec.h"
#include "crpc/core/net/io_service_pool.h"

tcp_client::~tcp_client()
{
    stop();
}

tcp_client& tcp_client::connect(const std::string& connect_address)
{
    connect_address_ = connect_address;
    return *this;
}

tcp_client& tcp_client::connect_timeout(time_t connect_timeout)
{
    connect_timeout_ = connect_timeout;
    return *this;
}

tcp_client& tcp_client::request_timeout(time_t request_timeout)
{
    request_timeout_ = request_timeout;
    return *this;
}

tcp_client& tcp_client::resend(bool resend)
{
    resend_ = resend;
    return *this;
}

bool tcp_client::run()
{
    pool_ = std::make_shared<io_service_pool>();
    pool_->init_pool_size(1);
    pool_->run();

    if (!parse_network_address())
    {
        return false;
    }

    session_ = std::make_shared<tcp_session>(codec_, pool_->get_io_service());
    session_->set_session_status_callback(std::bind(&tcp_client::session_status_callback, this, 
                                                    std::placeholders::_1, std::placeholders::_2));
    if (connect(session_->get_socket()))
    {
        session_->run();
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

void tcp_client::set_session_status_callback(const std::function<void(session_status, const std::string&)>& func)
{
    session_status_callback_ = func;
}

bool tcp_client::parse_network_address()
{
    std::string ip;
    unsigned short port = 0;
    if (get_ip_and_port(connect_address_, ip, port))
    {
        boost::asio::ip::tcp::resolver resolver(pool_->get_io_service());
        boost::asio::ip::tcp::resolver::query query(boost::asio::ip::tcp::v4(), ip, std::to_string(port));
        endpoint_iter_ = resolver.resolve(query);
        return true;
    }

    log_warn() << "parse address failed, address: " << connect_address_;
    return false;
}

bool tcp_client::connect(boost::asio::ip::tcp::socket& socket)
{
    static const time_t connect_timeout = 3;
    time_t begin_time = time(nullptr);
    while (true)
    {
        try
        {
            boost::asio::connect(socket, endpoint_iter_);
            return true;
        }
        catch (std::exception& e)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
            if (time(nullptr) - begin_time >= connect_timeout)
            {
                return false;
            }
        }
    }
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
        }
        else if (ec != boost::asio::error::already_connected)
        {
            reconnect();
        }
    });
}

void tcp_client::session_status_callback(session_status status, const std::string& session_id)
{
    if (session_status_callback_)
    {
        session_status_callback_(status, session_id);
    }

    if (status == session_status::closed)
    {
        reconnect();
    }
}
