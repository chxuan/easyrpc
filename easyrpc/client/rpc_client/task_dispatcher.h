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
#include "easyrpc/utility/atimer.h"
#include "easyrpc/utility/thread_pool.h"
#include "easyrpc/utility/qt_connect.h"
#include "easyrpc/client/rpc_client/task.h"

class task_dispatcher
{
public:
    task_dispatcher(time_t request_timeout);
    ~task_dispatcher();

    void add_result_handler(int serial_num, const result_handler& handler);
    void clear();
    void stop();

private slots:
    void handle_complete_client_decode_data(const std::shared_ptr<result>& ret);

private:
    void dispatch_thread(const std::shared_ptr<result>& ret);
    void check_request_timeout();

private:
    time_t request_timeout_;
    std::unordered_map<int, task> tasks_;
    std::mutex mutex_;
    atimer<boost::posix_time::seconds> timer_;
    thread_pool threadpool_;
};
