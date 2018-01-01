/**
 * @file task_dispatcher.h
 * @brief rpc任务调度器
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-07
 */
#pragma once

#include <unordered_map>
#include "task.h"
#include "easyrpc/utility/atimer.h"
#include "easyrpc/utility/thread_pool.h"
#include "easyrpc/utility/lock_shared.h"

class task_dispatcher
{
public:
    task_dispatcher(int request_timeout);
    ~task_dispatcher();

    void run();
    void add_result_handler(int serial_num, const result_handler& handler);
    void bind(const sub_handler& handler);
    void stop();
    void clear();
    void dispatch(const std::shared_ptr<result>& ret);

private:
    void dispatch_thread(const std::shared_ptr<result>& ret);
    void deal_rpc_result(const std::shared_ptr<result>& ret);
    void deal_sub_result(const std::shared_ptr<result>& ret);
    void check_request_timeout();
    bool get_task(int serial_num, task& t);
    void remove_task(int serial_num);

private:
    time_t request_timeout_;
    std::unordered_map<int, task> tasks_;
    shared_mutex mutex_;
    atimer<boost::posix_time::seconds> timer_;
    thread_pool threadpool_;
    sub_handler sub_handler_;
};
