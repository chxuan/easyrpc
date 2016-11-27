#ifndef _CONNECTION_H
#define _CONNECTION_H

#include <vector>
#include <memory>
#include <boost/asio.hpp>
#include <boost/timer.hpp>
#include "base/header.hpp"
#include "base/atimer.hpp"
#include "base/scope_guard.hpp"
#include "base/logger.hpp"
#include "router.hpp"

namespace easyrpc
{

class connection : public std::enable_shared_from_this<connection>
{
public:
    friend class router;
    connection() = default;
    connection(const connection&) = delete;
    connection& operator=(const connection&) = delete;
    connection(boost::asio::io_service& ios, std::size_t timeout_milli = 0)
        : socket_(ios), timer_(ios), timeout_milli_(timeout_milli) {}

    ~connection()
    {
        stop_timer();
        disconnect();
    }

    void start()
    {
        set_no_delay();
        read_head();
    }

    boost::asio::ip::tcp::socket& socket()
    {
        return socket_;
    }

    void write(const std::string& body)
    {
        unsigned int body_len = static_cast<unsigned int>(body.size());
        if (body_len > max_buffer_len)
        {
            throw std::runtime_error("Send data is too big");
        }

        const auto& buffer = get_buffer(response_header{ body_len }, body);
        write_impl(buffer);
    }

    void disconnect()
    {
        if (socket_.is_open())
        {
            boost::system::error_code ignore_ec;
            socket_.shutdown(boost::asio::socket_base::shutdown_both, ignore_ec);
            socket_.close(ignore_ec);
        }
    }

private:
    void read_head()
    {
        start_timer();
        auto self(this->shared_from_this());
        boost::asio::async_read(socket_, boost::asio::buffer(head_), 
                                [this, self](boost::system::error_code ec, std::size_t)
        {
            auto guard = make_guard([this, self]{ stop_timer(); disconnect(); });
            if (!socket_.is_open())
            {
                log_warn("Socket is not open");
                return;
            }

            if (ec)
            {
                log_warn(ec.message());
                return;
            }

            if (check_head())
            {
                read_protocol_and_body();
                guard.dismiss();
            }
        });
    }

    bool check_head()
    {
        memcpy(&req_head_, head_, sizeof(head_));
        unsigned int len = req_head_.protocol_len + req_head_.body_len;
        return (len > 0 && len < max_buffer_len) ? true : false;
    }

    void read_protocol_and_body()
    {
        protocol_and_body_.clear();
        protocol_and_body_.resize(req_head_.protocol_len + req_head_.body_len);
        auto self(this->shared_from_this());
        boost::asio::async_read(socket_, boost::asio::buffer(protocol_and_body_), 
                                [this, self](boost::system::error_code ec, std::size_t)
        {
            stop_timer();
            if (!socket_.is_open())
            {
                log_warn("Socket is not open");
                return;
            }

            auto guard = make_guard([this, self]{ disconnect(); });
            if (ec)
            {
                log_warn(ec.message());
                return;
            }

            bool ok = router::instance().route(std::string(&protocol_and_body_[0], req_head_.protocol_len), 
                                               std::string(&protocol_and_body_[req_head_.protocol_len], req_head_.body_len), 
                                               req_head_.mode, self);
            if (!ok)
            {
                log_warn("Router failed");
                return;
            }
            guard.dismiss();
        });
    }

    void set_no_delay()
    {
        boost::asio::ip::tcp::no_delay option(true);
        boost::system::error_code ec;
        socket_.set_option(option, ec);
    }

    void start_timer()
    {
        if (timeout_milli_ == 0)
        {
            return;
        }

        auto self(this->shared_from_this());
        timer_.bind([this, self]{ disconnect(); });
        timer_.set_single_shot(true);
        timer_.start(timeout_milli_);
    }

    void stop_timer()
    {
        if (timeout_milli_ == 0)
        {
            return;
        }
        timer_.stop();
    }

    std::vector<boost::asio::const_buffer> get_buffer(const response_header& head, const std::string& body)
    {
        std::vector<boost::asio::const_buffer> buffer;
        buffer.emplace_back(boost::asio::buffer(&head, sizeof(response_header)));
        buffer.emplace_back(boost::asio::buffer(body));
        return buffer;
    }

    void write_impl(const std::vector<boost::asio::const_buffer>& buffer)
    {
        boost::system::error_code ec;
        boost::asio::write(socket_, buffer, ec);
        if (ec)
        {
            throw std::runtime_error(ec.message());
        }
    }

private:
    boost::asio::ip::tcp::socket socket_;
    char head_[request_header_len];
    request_header req_head_;
    std::vector<char> protocol_and_body_;
    atimer<> timer_;
    std::size_t timeout_milli_ = 0;
};

}

#endif
