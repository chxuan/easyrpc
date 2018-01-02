/**
 * @file tcp_server.h
 * @brief tcp服务端
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-16
 */
#pragma once

#include <atomic>
#include <memory>
#include <functional>
#include <boost/asio.hpp>
#include <google/protobuf/message.h>
#include "easyrpc/utility/utiltiy.h"

class request;
class response;
class io_service_pool;
class tcp_session;
class tcp_session_cache;

struct connection_status
{
    bool created;
    std::string session_id;
    int connection_counts;
};

using notify_handler = std::function<void(const connection_status&)>;

class tcp_server
{
public:
    tcp_server(const std::string& host, int ios_threads);
    virtual ~tcp_server();

    void set_connection_notify(const notify_handler& func);
    void publish(const std::string& session_id, const std::shared_ptr<google::protobuf::Message>& message);
    virtual bool run();
    virtual void stop();

protected:
    virtual void deal_request(const std::shared_ptr<request>& req, const std::shared_ptr<response>& rsp) = 0;

private:
    bool start_listen();
    bool listen(const std::string& ip, unsigned short port);
    void accept();
    void deal_connection_created(const std::shared_ptr<tcp_session>& session);
    void deal_connection_closed(const std::string& session_id);
    int make_serial_num();

private:
    std::string host_;
    std::shared_ptr<tcp_session_cache> session_cache_;
    std::shared_ptr<io_service_pool> pool_;
    boost::asio::ip::tcp::acceptor acceptor_;
    notify_handler notify_func_;
    std::atomic<int> connection_counts_{ 0 };
};
