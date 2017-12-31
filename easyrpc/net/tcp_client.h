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
class tcp_session;
class io_service_pool;

class tcp_client
{
public:
    tcp_client(const std::string& address);
    virtual ~tcp_client();

    void set_session_status_callback(const std::function<void(bool, const std::string&)>& func);
    virtual bool run();
    virtual void stop();
    void async_write(const std::shared_ptr<std::string>& network_data);


private:
    bool parse_connect_address();
    bool connect(boost::asio::ip::tcp::socket& socket);
    void reconnect();
    void deal_session_established();
    void deal_session_closed(const std::string& session_id);

protected:
    std::shared_ptr<codec> codec_;

private:
    std::string address_;
    std::shared_ptr<io_service_pool> pool_;
    std::shared_ptr<tcp_session> session_;
    boost::asio::ip::tcp::resolver::iterator endpoint_iter_;
    std::function<void(bool, const std::string&)> session_status_callback_;
};
