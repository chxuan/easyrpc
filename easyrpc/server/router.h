/**
 * @file router.h
 * @brief rpc服务消息路由器
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-19
 */
#pragma once

#include <unordered_map>
#include "easyrpc/utility/thread_pool.h"

class request;
class response;

using request_handler = std::function<void(const std::shared_ptr<request>&, const std::shared_ptr<response>&)>;

class router
{
public:
    ~router();

    void run(int work_threads);
    std::size_t route_table_size();
    void bind(const std::string& message_name, const request_handler& func);
    void stop();
    void route(const std::shared_ptr<request>& req, const std::shared_ptr<response>& rsp);

private:
    void route_thread(const std::shared_ptr<request>& req, const std::shared_ptr<response>& rsp);

private:
    thread_pool threadpool_;
    std::unordered_map<std::string, request_handler> route_table_;
};
