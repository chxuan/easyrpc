/**
 * @file rpc_client_test.h
 * @brief rpc客户端测试程序
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-12-02
 */
#pragma once

#include <memory>
#include <google/protobuf/message.h>

class rpc_client;

class rpc_client_test
{
public:
    ~rpc_client_test();
    
    void run();
    void stop();

private:
    void session_status_callback(bool established, const std::string& session_id);
    void received_sub_message(const std::shared_ptr<google::protobuf::Message>& message);
    void call();

private:
    std::shared_ptr<rpc_client> client_;
};
