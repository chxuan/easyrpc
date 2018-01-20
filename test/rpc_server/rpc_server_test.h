/**
 * @file rpc_server_test.h
 * @brief rpc服务端测试程序
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-12-02
 */
#pragma once

#include <thread>
#include "../protoc/code/common.pb.h"
#include "easyrpc/easyrpc.h"

using namespace easyrpc;

class rpc_server_test
{
public:
    rpc_server_test();
    ~rpc_server_test();

    void run();
    void stop();

private:
    void deal_connection_notify(const connection_status& status);
    void echo(const std::shared_ptr<request>& req, const std::shared_ptr<response>& res);
    void publish_thread();
    std::shared_ptr<google::protobuf::Message> make_auto_weather();

private:
    std::shared_ptr<rpc_server> server_;
    std::shared_ptr<std::thread> pub_thread_;
    std::atomic<bool> server_stoped_{ false };
    std::string client_session_id_;
};
