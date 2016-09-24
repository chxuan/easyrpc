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
        : _socket(ios), _timer(ios), _timeout_milli(timeout_milli) {}

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
        return _socket;
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
        if (_socket.is_open())
        {
            boost::system::error_code ignore_ec;
            _socket.shutdown(boost::asio::socket_base::shutdown_both, ignore_ec);
            _socket.close(ignore_ec);
        }
    }

private:
    void read_head()
    {
        start_timer();
        auto self(this->shared_from_this());
        boost::asio::async_read(_socket, boost::asio::buffer(_head), 
                                [this, self](boost::system::error_code ec, std::size_t)
        {
            auto guard = make_guard([this, self]{ stop_timer(); disconnect(); });
            if (!_socket.is_open())
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
        memcpy(&_req_head, _head, sizeof(_head));
        unsigned int len = _req_head.protocol_len + _req_head.body_len;
        return (len > 0 && len < max_buffer_len) ? true : false;
    }

    void read_protocol_and_body()
    {
        _protocol_and_body.clear();
        _protocol_and_body.resize(_req_head.protocol_len + _req_head.body_len);
        auto self(this->shared_from_this());
        boost::asio::async_read(_socket, boost::asio::buffer(_protocol_and_body), 
                                [this, self](boost::system::error_code ec, std::size_t)
        {
            stop_timer();
            if (!_socket.is_open())
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

            bool ok = router::instance().route(std::string(&_protocol_and_body[0], _req_head.protocol_len), 
                                               std::string(&_protocol_and_body[_req_head.protocol_len], _req_head.body_len), 
                                               _req_head.mode, self);
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
        _socket.set_option(option, ec);
    }

    void start_timer()
    {
        if (_timeout_milli == 0)
        {
            return;
        }

        auto self(this->shared_from_this());
        _timer.bind([this, self]{ disconnect(); });
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
        boost::asio::write(_socket, buffer, ec);
        if (ec)
        {
            throw std::runtime_error(ec.message());
        }
    }

private:
    boost::asio::ip::tcp::socket _socket;
    char _head[request_header_len];
    request_header _req_head;
    std::vector<char> _protocol_and_body;
    atimer<> _timer;
    std::size_t _timeout_milli = 0;
};

}

#endif
