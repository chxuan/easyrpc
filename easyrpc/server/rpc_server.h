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
    rpc_server(const std::string& host, int ios_threads = 4, int work_threads = 4);
    virtual ~rpc_server();

    virtual bool run();
    virtual void stop();
    void bind(const std::string& message_name, const request_handler& func);

protected:
    virtual void deal_request(const std::shared_ptr<request>& req, const std::shared_ptr<response>& res);

private:
    int work_threads_;
    router router_;
};
