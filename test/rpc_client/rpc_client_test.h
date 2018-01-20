/**
 * @file rpc_client_test.h
 * @brief rpc客户端测试程序
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-12-02
 */
#pragma once

#include "../protoc/code/common.pb.h"
#include "easyrpc/easyrpc.h"

using namespace easyrpc;

class rpc_client_test
{
public:
    rpc_client_test();
    ~rpc_client_test();
    
    void run();
    void stop();

private:
    void deal_connection_notify(bool created, const std::string& session_id);
    void deal_sub_message(const std::shared_ptr<result>& ret);
    void call();
    std::shared_ptr<google::protobuf::Message> make_echo_message();

private:
    std::shared_ptr<rpc_client> client_;
};
