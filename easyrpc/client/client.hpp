#ifndef _CLIENT_H
#define _CLIENT_H

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <thread>
#include <memory>
#include <iostream>
#include <boost/asio.hpp>
#include "base/header.hpp"
#include "base/atimer.hpp"
#include "base/scope_guard.hpp"
#include "base/string_util.hpp"
#include "protocol.hpp"

namespace easyrpc
{

class client
{
public:
    client(const client&) = delete;
    client& operator=(const client&) = delete;
    client() : work_(ios_), socket_(ios_), 
    timer_work_(timer_ios_), timer_(timer_ios_) {}

    ~client()
    {
        stop();
    }

    client& connect(const std::string& address)
    {
        std::vector<std::string> token = string_util::split(address, ":");
        if (token.size() != 2)
        {
            throw std::invalid_argument("Address format error");
        }
        return connect(token[0], token[1]);
    }

    client& connect(const std::string& ip, unsigned short port)
    {
        return connect(ip, std::to_string(port));
    }

    client& connect(const std::string& ip, const std::string& port)
    {
        boost::asio::ip::tcp::resolver resolver(ios_);
        boost::asio::ip::tcp::resolver::query query(boost::asio::ip::tcp::v4(), ip, port);
        endpoint_iter_ = resolver.resolve(query);
        return *this;
    }

    client& timeout(std::size_t timeout_milli)
    {
        timeout_milli_ = timeout_milli;
        return *this;
    }

    void run()
    {
        thread_ = std::make_unique<std::thread>([this]{ ios_.run(); });
        if (timeout_milli_ != 0)
        {
            timer_thread_ = std::make_unique<std::thread>([this]{ timer_ios_.run(); });
        }
    }

    void stop()
    {
        stop_ios_thread();
        stop_timer_thread();
    }

    template<typename Protocol, typename... Args>
    typename std::enable_if<std::is_void<typename Protocol::return_type>::value, typename Protocol::return_type>::type
    call(const Protocol& protocol, Args&&... args)
    {
        connect();
        auto guard = make_guard([this]{ disconnect(); });
        // 读取到buf后不进行任何处理，因为client建立的短连接.
        // 需要server端进行确认后才能断开连接.
        call_impl(protocol.name(), call_mode::non_raw, protocol.pack(std::forward<Args>(args)...));
    }

    template<typename Protocol, typename... Args>
    typename std::enable_if<!std::is_void<typename Protocol::return_type>::value, typename Protocol::return_type>::type
    call(const Protocol& protocol, Args&&... args)
    {
        connect();
        auto guard = make_guard([this]{ disconnect(); });
        call_impl(protocol.name(), call_mode::non_raw, protocol.pack(std::forward<Args>(args)...));
        return protocol.unpack(std::string(&body_[0], body_.size()));
    }

    template<typename ReturnType>
    typename std::enable_if<std::is_same<ReturnType, one_way>::value>::type 
    call_raw(const std::string& protocol, const std::string& body)
    {
        connect();
        auto guard = make_guard([this]{ disconnect(); });
        call_impl(protocol, call_mode::raw, body);
    }

    template<typename ReturnType>
    typename std::enable_if<std::is_same<ReturnType, two_way>::value, std::string>::type 
    call_raw(const std::string& protocol, const std::string& body)
    {
        connect();
        auto guard = make_guard([this]{ disconnect(); });
        call_impl(protocol, call_mode::raw, body);
        return std::string(&body_[0], body_.size());
    }

private:
    void connect()
    {
        boost::asio::connect(socket_, endpoint_iter_);
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

    void call_impl(const std::string& protocol, const call_mode& mode, const std::string& body)
    {
        write(protocol, mode, body);
        read();
    }

    void write(const std::string& protocol, const call_mode& mode, const std::string& body)
    {
        unsigned int protocol_len = static_cast<unsigned int>(protocol.size());
        unsigned int body_len = static_cast<unsigned int>(body.size());
        if (protocol_len + body_len > max_buffer_len)
        {
            throw std::runtime_error("Send data is too big");
        }

        const auto& buffer = get_buffer(request_header{ protocol_len, body_len, mode }, protocol, body);
        write_impl(buffer);
    }

    std::vector<boost::asio::const_buffer> get_buffer(const request_header& head, 
                                                      const std::string& protocol, 
                                                      const std::string& body)
    {
        std::vector<boost::asio::const_buffer> buffer;
        buffer.emplace_back(boost::asio::buffer(&head, sizeof(request_header)));
        buffer.emplace_back(boost::asio::buffer(protocol));
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

    void read()
    {
        start_timer();
        auto guard = make_guard([this]{ stop_timer(); });
        read_head();
        check_head();
        read_body();
    }

    void read_head()
    {
        boost::system::error_code ec;
        boost::asio::read(socket_, boost::asio::buffer(head_), ec);
        if (ec)
        {
            throw std::runtime_error(ec.message());
        }
    }

    void check_head()
    {
        memcpy(&_res_head, head_, sizeof(head_));
        if (_res_head.body_len > max_buffer_len)
        {
            throw std::runtime_error("Body len is too big");
        }
    }

    void read_body()
    {
        body_.clear();
        body_.resize(_res_head.body_len);
        boost::system::error_code ec;
        boost::asio::read(socket_, boost::asio::buffer(body_), ec); 
        if (ec)
        {
            throw std::runtime_error(ec.message());
        }
    }

    void start_timer()
    {
        if (timeout_milli_ == 0)
        {
            return;
        }

        timer_.bind([this]{ disconnect(); });
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

    void stop_ios_thread()
    {
        ios_.stop();
        if (thread_ != nullptr)
        {
            if (thread_->joinable())
            {
                thread_->join();
            }
        }
    }

    void stop_timer_thread()
    {
        timer_ios_.stop();
        if (timer_thread_ != nullptr)
        {
            if (timer_thread_->joinable())
            {
                timer_thread_->join();
            }
        }
    }

private:
    boost::asio::io_service ios_;
    boost::asio::io_service::work work_;
    boost::asio::ip::tcp::socket socket_;
    boost::asio::ip::tcp::resolver::iterator endpoint_iter_;
    std::unique_ptr<std::thread> thread_;
    char head_[response_header_len];
    response_header _res_head;
    std::vector<char> body_;
    boost::asio::io_service timer_ios_;
    boost::asio::io_service::work timer_work_;
    std::unique_ptr<std::thread> timer_thread_;
    atimer<> timer_;
    std::size_t timeout_milli_ = 0;
};

}

#endif
