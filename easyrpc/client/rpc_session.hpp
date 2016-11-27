#ifndef _RPC_SESSION_H
#define _RPC_SESSION_H

#include <string>
#include <vector>
#include <thread>
#include <memory>
#include <boost/asio.hpp>
#include "base/header.hpp"
#include "base/atimer.hpp"
#include "base/scope_guard.hpp"

namespace easyrpc
{

class rpc_session
{
public:
    rpc_session(const rpc_session&) = delete;
    rpc_session& operator=(const rpc_session&) = delete;
    rpc_session() : work_(ios_), socket_(ios_), 
    timer_work_(timer_ios_), timer_(timer_ios_) {}

    ~rpc_session()
    {
        stop();
    }

    void connect(const std::string& ip, const std::string& port)
    {
        boost::asio::ip::tcp::resolver resolver(ios_);
        boost::asio::ip::tcp::resolver::query query(boost::asio::ip::tcp::v4(), ip, port);
        endpoint_iter_ = resolver.resolve(query);
    }

    void timeout(std::size_t timeout_milli)
    {
        timeout_milli_ = timeout_milli;
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

    std::vector<char> call(const std::string& protocol, const call_mode& mode, const std::string& body)
    {
        write(protocol, mode, body);
        return read();
    }

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

private:
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

    std::vector<char> read()
    {
        start_timer();
        auto guard = make_guard([this]{ stop_timer(); });
        read_head();
        check_head();
        return read_body();
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

    std::vector<char> read_body()
    {
        body_.clear();
        body_.resize(_res_head.body_len);
        boost::system::error_code ec;
        boost::asio::read(socket_, boost::asio::buffer(body_), ec); 
        if (ec)
        {
            throw std::runtime_error(ec.message());
        }
        return body_;
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
