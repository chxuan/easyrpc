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
    client() : _work(_ios), _socket(_ios), 
    _timer_work(_timer_ios), _timer(_timer_ios) {}

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
        boost::asio::ip::tcp::resolver resolver(_ios);
        boost::asio::ip::tcp::resolver::query query(boost::asio::ip::tcp::v4(), ip, port);
        _endpoint_iter = resolver.resolve(query);
        return *this;
    }

    client& timeout(std::size_t timeout_milli)
    {
        _timeout_milli = timeout_milli;
        return *this;
    }

    void run()
    {
        _thread = std::make_unique<std::thread>([this]{ _ios.run(); });
        if (_timeout_milli != 0)
        {
            _timer_thread = std::make_unique<std::thread>([this]{ _timer_ios.run(); });
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
        return protocol.unpack(std::string(&_body[0], _body.size()));
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
        return std::string(&_body[0], _body.size());
    }

private:
    void connect()
    {
        boost::asio::connect(_socket, _endpoint_iter);
    }

    void disconnect()
    {
        if (_socket.is_open())
        {
            boost::system::error_code ignore_ec;
            _socket.shutdown(boost::asio::socket_base::shutdown_both, ignore_ec);
            _socket.close(ignore_ec);
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
        boost::asio::write(_socket, buffer, ec);
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
        boost::asio::read(_socket, boost::asio::buffer(_head), ec);
        if (ec)
        {
            throw std::runtime_error(ec.message());
        }
    }

    void check_head()
    {
        memcpy(&_res_head, _head, sizeof(_head));
        if (_res_head.body_len > max_buffer_len)
        {
            throw std::runtime_error("Body len is too big");
        }
    }

    void read_body()
    {
        _body.clear();
        _body.resize(_res_head.body_len);
        boost::system::error_code ec;
        boost::asio::read(_socket, boost::asio::buffer(_body), ec); 
        if (ec)
        {
            throw std::runtime_error(ec.message());
        }
    }

    void start_timer()
    {
        if (_timeout_milli == 0)
        {
            return;
        }

        _timer.bind([this]{ disconnect(); });
        _timer.set_single_shot(true);
        _timer.start(_timeout_milli);
    }

    void stop_timer()
    {
        if (_timeout_milli == 0)
        {
            return;
        }
        _timer.stop();
    }

    void stop_ios_thread()
    {
        _ios.stop();
        if (_thread != nullptr)
        {
            if (_thread->joinable())
            {
                _thread->join();
            }
        }
    }

    void stop_timer_thread()
    {
        _timer_ios.stop();
        if (_timer_thread != nullptr)
        {
            if (_timer_thread->joinable())
            {
                _timer_thread->join();
            }
        }
    }

private:
    boost::asio::io_service _ios;
    boost::asio::io_service::work _work;
    boost::asio::ip::tcp::socket _socket;
    boost::asio::ip::tcp::resolver::iterator _endpoint_iter;
    std::unique_ptr<std::thread> _thread;
    char _head[response_header_len];
    response_header _res_head;
    std::vector<char> _body;
    boost::asio::io_service _timer_ios;
    boost::asio::io_service::work _timer_work;
    std::unique_ptr<std::thread> _timer_thread;
    atimer<> _timer;
    std::size_t _timeout_milli = 0;
};

}

#endif
