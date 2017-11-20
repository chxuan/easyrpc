/**
 * @file rpc_client.h
 * @brief 异步rpc客户端
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-06
 */
#pragma once

#include "crpc/core/net/tcp_client.h"
#include "crpc/client/rpc_client/result.h"

class task_dispatcher;

class rpc_client : public tcp_client
{
public:
    rpc_client();
    virtual ~rpc_client();

    virtual bool run();
    virtual void stop();
    unsigned int call(const std::string& func_name, 
                      const std::shared_ptr<google::protobuf::Message>& message, 
                      const std::function<void(const std::shared_ptr<result>&)>& func);

private:
    auto make_recv_handler(const std::function<void(const std::shared_ptr<result>&)>& func);
    unsigned int make_serial_num();
    void decode_data_callback(const response_content& body);

private:
    std::shared_ptr<task_dispatcher> dispatcher_ = nullptr;
};
