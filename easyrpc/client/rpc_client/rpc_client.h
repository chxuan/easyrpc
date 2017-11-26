/**
 * @file rpc_client.h
 * @brief 异步rpc客户端
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-06
 */
#pragma once

#include "easyrpc/core/net/tcp_client.h"
#include "easyrpc/client/rpc_client/result.h"

class task_dispatcher;

class rpc_client : public tcp_client
{
public:
    rpc_client();
    virtual ~rpc_client();

    virtual bool run();
    virtual void stop();
    int call(int func_id,
             const std::shared_ptr<google::protobuf::Message>& message, 
             const std::function<void(const std::shared_ptr<result>&)>& func);

private:
    auto make_result_handler(const std::function<void(const std::shared_ptr<result>&)>& func);
    int make_serial_num();

private:
    std::shared_ptr<task_dispatcher> dispatcher_ = nullptr;
};
