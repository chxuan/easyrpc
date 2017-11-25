/**
 * @file tcp_session.h
 * @brief tcp会话
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-05
 */
#pragma once

#include <boost/asio.hpp>
#include "easyrpc/utility/threadsafe_list.h"

class codec;

class tcp_session : public std::enable_shared_from_this<tcp_session>
{
public:
    tcp_session(const std::shared_ptr<codec>& dec, boost::asio::io_service& ios);
    ~tcp_session();

    void run();
    void close();
    boost::asio::io_service& get_io_service();
    boost::asio::ip::tcp::socket& get_socket();
    std::string get_session_id();
    void async_write(const std::shared_ptr<std::string>& network_data);

private:
    void async_write_loop();
    void async_read();
    void set_no_delay();
    void resize_buffer(int size);
    void handle_session_closed();

private:
    std::shared_ptr<codec> codec_;
    boost::asio::io_service& ios_;
    boost::asio::ip::tcp::socket socket_;
    threadsafe_list<std::shared_ptr<std::string>> send_queue_;
    std::vector<char> buffer_;
    std::atomic<bool> established_{ false };
    std::string session_id_;
};
