/**
 * @file rpc_client.h
 * @brief 异步rpc客户端
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-06
 */
#pragma once

#include "easyrpc/net/tcp_client.h"
#include "result.h"
#include "task.h"

namespace easyrpc
{

class task_dispatcher;

class rpc_client : public tcp_client
{
public:
    rpc_client(const std::string& address, int request_timeout = 10);
    virtual ~rpc_client();

    virtual bool run();
    virtual void stop();
    int call(const std::shared_ptr<google::protobuf::Message>& message, const result_handler& handler);
    void bind(const sub_handler& handler);

private:
    int make_serial_num();

private:
    std::shared_ptr<task_dispatcher> dispatcher_;
};

}
