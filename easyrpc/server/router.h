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
#include "easyrpc/utility/qt_connect.h"

class request;
class response;

using function_t = std::function<void(const std::shared_ptr<request>&, 
                                      const std::shared_ptr<response>&)>;

class router
{
public:
    router();
    ~router();

    void run(int work_threads);
    std::size_t route_table_size();
    void bind(int func_id, const function_t& handler);
    void stop();

private slots:
    void handle_complete_server_decode_data(int func_id, 
                                            const std::shared_ptr<request>& req,
                                            const std::shared_ptr<response>& rsp);

private:
    void router_thread(int func_id, 
                       const std::shared_ptr<request>& req, 
                       const std::shared_ptr<response>& rsp);

private:
    thread_pool threadpool_;
    std::unordered_map<int, function_t> route_table_;
};
