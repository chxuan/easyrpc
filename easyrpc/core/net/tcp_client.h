/**
 * @file tcp_client.h
 * @brief tcp客户端基类
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-05
 */
#pragma once

#include <boost/asio.hpp>
#include "easyrpc/utility/qt_connect.h"

class codec;
class io_service_pool;
class tcp_session;

class tcp_client
{
public:
    tcp_client();
    virtual ~tcp_client();

    tcp_client& connect(const std::string& address);
    tcp_client& connect_timeout(time_t seconds);
    tcp_client& request_timeout(time_t seconds);
    tcp_client& resend(bool resend);
    void set_session_status_callback(const std::function<void(bool, const std::string&)>& func);

    virtual bool run();
    virtual void stop();
    void async_write(const std::shared_ptr<std::string>& network_data);

private slots:
    void handle_session_status_changed(bool established, const std::string& session_id);

private:
    void create_io_service_pool();
    bool parse_network_address();
    bool connect(boost::asio::ip::tcp::socket& socket);
    void reconnect();

protected:
    std::shared_ptr<codec> codec_;
    time_t request_timeout_ = 10;

private:
    std::string connect_address_;
    time_t connect_timeout_ = 3;
    std::shared_ptr<io_service_pool> pool_;
    std::shared_ptr<tcp_session> session_;
    std::atomic<bool> resend_{ false };
    boost::asio::ip::tcp::resolver::iterator endpoint_iter_;
    std::function<void(bool, const std::string&)> session_status_callback_ = nullptr;
};
