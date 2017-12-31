/**
 * @file tcp_server.h
 * @brief tcp服务端
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-16
 */
#pragma once

#include <memory>
#include <functional>
#include <boost/asio.hpp>
#include <google/protobuf/message.h>
#include "easyrpc/utility/utiltiy.h"
#include "easyrpc/utility/qt_connect.h"

class io_service_pool;
class tcp_session;
class tcp_session_cache;

class tcp_server
{
public:
    tcp_server(const std::string& host, int ios_threads);
    virtual ~tcp_server();

    void set_session_status_callback(const std::function<void(bool, const std::string&)>& func);
    void publish(const std::string& session_id, const std::shared_ptr<google::protobuf::Message>& message);
    virtual bool run();
    virtual void stop();

private:
    bool start_listen();
    bool listen(const std::string& ip, unsigned short port);
    void accept();
    void deal_session_closed(const std::string& session_id);
    void deal_session_established(const std::shared_ptr<tcp_session>& session);

private:
    std::string host_;
    std::shared_ptr<tcp_session_cache> session_cache_;
    std::shared_ptr<io_service_pool> pool_;
    boost::asio::ip::tcp::acceptor acceptor_;
    std::function<void(bool, const std::string&)> session_status_callback_;
};
