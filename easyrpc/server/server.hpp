#ifndef _SERVER_H
#define _SERVER_H

#include "io_service_pool.hpp"
#include "router.hpp"
#include "connection.hpp"
#include "base/string_util.hpp"

namespace easyrpc
{

class server
{
public:
    server(const server&) = delete;
    server& operator=(const server&) = delete;
    server() : ios_pool_(std::thread::hardware_concurrency()), 
    acceptor_(ios_pool_.get_io_service()) {}

    ~server()
    {
        stop();
    }

    server& listen(const std::string& address)
    {
        if (string_util::contains(address, ":"))
        {
            std::vector<std::string> token = string_util::split(address, ":");
            if (token.size() != 2)
            {
                throw std::invalid_argument("Address format error");
            }
            if (string_util::equals_ignore_case(token[0], "localhost"))
            {
                return listen("127.0.0.1", token[1]);
            }
            return listen(token[0], token[1]);
        }
        // is port
        return listen("0.0.0.0", address);    
    }

    server& listen(unsigned short port)
    {
        return listen("0.0.0.0", port);
    }

    server& listen(const std::string& ip, const std::string& port)
    {
        return listen(ip, static_cast<unsigned short>(std::stoi(port)));
    }

    server& listen(const std::string& ip, unsigned short port)
    {
        ip_ = ip;
        port_ = port;
        return *this;
    }

    server& timeout(std::size_t timeout_milli)
    {
        timeout_milli_ = timeout_milli;
        return *this;
    }

    server& multithreaded(std::size_t num)
    {
        thread_num_ = num;
        return *this;
    }

    void run()
    {
        router::instance().multithreaded(thread_num_);
        listen();
        accept();
        ios_pool_.run();
    }

    void stop()
    {
        ios_pool_.stop();
    }

    template<typename Function>
    void bind(const std::string& protocol, const Function& func)
    {
        router::instance().bind(protocol, func);
    }

    template<typename Function, typename Self>
    void bind(const std::string& protocol, const Function& func, Self* self)
    {
        router::instance().bind(protocol, func, self); 
    }

    void unbind(const std::string& protocol)
    {
        router::instance().unbind(protocol);
    }

    bool is_bind(const std::string& protocol)
    {
        return router::instance().is_bind(protocol);
    }

    template<typename Function>
    void bind_raw(const std::string& protocol, const Function& func)
    {
        router::instance().bind_raw(protocol, func);
    }

    template<typename Function, typename Self>
    void bind_raw(const std::string& protocol, const Function& func, Self* self)
    {
        router::instance().bind_raw(protocol, func, self); 
    }

    void unbind_raw(const std::string& protocol)
    {
        router::instance().unbind_raw(protocol);
    }

    bool is_bind_raw(const std::string& protocol)
    {
        return router::instance().is_bind_raw(protocol);
    }

private:
    void listen()
    {
        boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address_v4::from_string(ip_), port_);
        acceptor_.open(ep.protocol());
        acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
        acceptor_.bind(ep);
        acceptor_.listen();
    }

    void accept()
    {
        std::shared_ptr<connection> conn = 
            std::make_shared<connection>(ios_pool_.get_io_service(), timeout_milli_);
        acceptor_.async_accept(conn->socket(), [this, conn](boost::system::error_code ec)
        {
            if (!ec)
            {
                conn->start();
            }
            accept();
        });
    }

private:
    io_service_pool ios_pool_;
    boost::asio::ip::tcp::acceptor acceptor_;
    std::string ip_ = "0.0.0.0";
    unsigned short port_ = 50051;
    std::size_t timeout_milli_ = 0;
    std::size_t thread_num_ = 1;
};

}

#endif
