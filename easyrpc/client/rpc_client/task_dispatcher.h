/**
 * @file task_dispatcher.h
 * @brief rpc任务调度器
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-07
 */
#pragma once

#include <unordered_map>
#include <mutex>
#include "crpc/utility/atimer.h"
#include "crpc/utility/thread_pool.h"
#include "crpc/client/rpc_client/task.h"

class task_dispatcher
{
public:
    task_dispatcher(time_t request_timeout);
    ~task_dispatcher();

    void add_recv_handler(unsigned int serial_num, const recv_handler& handler);
    void dispatch(const response_content& content);
    void clear();
    void stop();

private:
    void check_request_timeout();

private:
    time_t request_timeout_;
    std::unordered_map<unsigned int, task> tasks_;
    std::mutex mutex_;
    atimer<boost::posix_time::seconds> timer_;
    thread_pool threadpool_;
};
