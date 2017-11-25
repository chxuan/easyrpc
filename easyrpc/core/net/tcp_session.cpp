#include "tcp_session.h"
#include "easyrpc/utility/logger.h"
#include "easyrpc/core/protocol/sig.h"
#include "easyrpc/core/codec/codec.h"

tcp_session::tcp_session(const std::shared_ptr<codec>& dec, boost::asio::io_service& ios) 
    : codec_(dec), 
    ios_(ios), 
    socket_(ios)
{

}

tcp_session::~tcp_session()
{
    close();
}

void tcp_session::run()
{
    set_no_delay();
    async_read();
    established_ = true;
    emit sig_session_status(established_, get_session_id());
}

boost::asio::io_service& tcp_session::get_io_service()
{
    return ios_;
}

boost::asio::ip::tcp::socket& tcp_session::get_socket()
{
    return socket_;
}

void tcp_session::close()
{
    established_ = false;
    if (socket_.is_open())
    {
        boost::system::error_code ignore_ec;
        socket_.shutdown(boost::asio::socket_base::shutdown_both, ignore_ec);
        socket_.close(ignore_ec);
    }
}

void tcp_session::async_write(const std::shared_ptr<std::string>& network_data)
{
    auto self(shared_from_this());
    ios_.post([this, self, network_data]
    {
        bool empty = send_queue_.empty();
        send_queue_.emplace_back(network_data);
        if (empty)
        {
            async_write_loop();
        }
    });
}

void tcp_session::async_write_loop()
{
    auto self(shared_from_this());
    boost::asio::async_write(socket_, boost::asio::buffer(*send_queue_.front()), 
                             [this, self](boost::system::error_code ec, std::size_t)
    {
        if (!ec)
        {
            send_queue_.pop_front();
            if (!send_queue_.empty())
            {
                async_write_loop();
            }
        }
        else
        {
            log_warn() << ec.message();
            send_queue_.clear();
        }
    });
}

void tcp_session::async_read()
{
    resize_buffer(codec_->get_next_recv_bytes());
    auto self(shared_from_this());
    boost::asio::async_read(socket_, boost::asio::buffer(buffer_), 
                            [this, self](boost::system::error_code ec, std::size_t)
    {
        if (!ec)
        {
            codec_->decode(buffer_);
            async_read();
        }
        else if (established_ && ec != boost::asio::error::operation_aborted)
        {
            handle_session_closed();
        }
    });
}

void tcp_session::set_no_delay()
{
    boost::asio::ip::tcp::no_delay option(true);
    boost::system::error_code ec;
    socket_.set_option(option, ec);
}

void tcp_session::resize_buffer(int size)
{
    buffer_.resize(size);
}

void tcp_session::handle_session_closed()
{
    close();
    emit sig_session_status(established_, get_session_id());
}

std::string tcp_session::get_session_id()
{
    if (session_id_.empty())
    {
        if (socket_.is_open())
        {
            boost::system::error_code ec, ec2;
            auto local_endpoint = socket_.local_endpoint();
            auto remote_endpoint = socket_.remote_endpoint();
            if (!ec && !ec2)
            {
                session_id_ = local_endpoint.address().to_string() + ":"
                    + std::to_string(local_endpoint.port()) + "#"
                    + remote_endpoint.address().to_string() + ":"
                    + std::to_string(remote_endpoint.port());
            }
        }
    }

    return session_id_;
}
