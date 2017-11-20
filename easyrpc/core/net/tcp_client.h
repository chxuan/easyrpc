/**
 * @file tcp_client.h
 * @brief tcp客户端基类
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-05
 */
#pragma once

#include "crpc/core/net/tcp_session.h"

class codec;
class io_service_pool;

class tcp_client
{
public:
    virtual ~tcp_client();

    tcp_client& connect(const std::string& connect_address);
    tcp_client& connect_timeout(time_t connect_timeout);
    tcp_client& request_timeout(time_t request_timeout);
    tcp_client& resend(bool resend);

    virtual bool run();
    virtual void stop();
    void async_write(const std::shared_ptr<std::string>& network_data);
    void set_session_status_callback(const std::function<void(session_status, const std::string&)>& func);

private:
    bool parse_network_address();
    bool connect(boost::asio::ip::tcp::socket& socket);
    void reconnect();
    void session_status_callback(session_status status, const std::string& session_id);

protected:
    std::shared_ptr<codec> codec_;
    time_t request_timeout_ = 10;

private:
    std::string connect_address_;
    time_t connect_timeout_ = 3;
    std::shared_ptr<io_service_pool> pool_;
    std::shared_ptr<tcp_session> session_;
    std::atomic<bool> resend_{ false };
    std::function<void()> closed_callback_;
    boost::asio::ip::tcp::resolver::iterator endpoint_iter_;
    std::function<void(session_status, const std::string&)> session_status_callback_ = nullptr;
};
