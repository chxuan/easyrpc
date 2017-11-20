/**
 * @file rpc_server.h
 * @brief rpc服务端
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-19
 */
#pragma once

#include "crpc/core/net/tcp_server.h"
#include "crpc/server/router.h"

class rpc_server : public tcp_server
{
public:
    virtual ~rpc_server();
    void bind(const std::string& id, const function_t& func);

private:
    router router_;
};
