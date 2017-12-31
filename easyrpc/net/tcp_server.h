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
class tcp_session_manager;

class tcp_server
{
public:
    tcp_server();
    virtual ~tcp_server();

    tcp_server& listen(const std::string& host);
    tcp_server& ios_threads(int num);
    tcp_server& work_threads(int num);
    void set_session_status_callback(const std::function<void(bool, const std::string&)>& func);
    void publish(const std::string& session_id, const std::shared_ptr<google::protobuf::Message>& message);

    virtual bool run();
    virtual void stop();

private slots:
    void deal_session_status_changed(bool established, const std::string& session_id);

private:
    bool start_listen();
    bool listen(const std::string& ip, unsigned short port);
    void accept();

protected:
    int work_threads_ = 4;

private:
    std::string host_;
    int ios_threads_ = 4;
    std::shared_ptr<tcp_session_manager> manager_;
    std::shared_ptr<io_service_pool> pool_;
    boost::asio::ip::tcp::acceptor acceptor_;
    std::function<void(bool, const std::string&)> session_status_callback_;
};
