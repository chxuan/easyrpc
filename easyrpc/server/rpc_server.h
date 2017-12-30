/**
 * @file rpc_server.h
 * @brief rpc服务端
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-19
 */
#pragma once

#include "easyrpc/net/tcp_server.h"
#include "router.h"

class rpc_server : public tcp_server
{
public:
    virtual ~rpc_server();

    virtual bool run();
    virtual void stop();

    void bind(const std::string& message_name, const function_t& handler);

private:
    router router_;
};
