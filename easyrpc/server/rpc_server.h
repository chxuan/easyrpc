/**
 * @file rpc_server.h
 * @brief rpc服务端
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-19
 */
#pragma once

#include "easyrpc/net/tcp_server.h"
#include "easyrpc/server/router.h"

class rpc_server : public tcp_server
{
public:
    virtual ~rpc_server();

    virtual bool run();
    virtual void stop();

    void bind(int func_id, const function_t& handler);

private:
    router router_;
};
