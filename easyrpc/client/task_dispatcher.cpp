#include "task_dispatcher.h"
#include "result.h"
#include "easyrpc/utility/logger.h"

task_dispatcher::task_dispatcher(int request_timeout)
    : request_timeout_(request_timeout)
{

}

task_dispatcher::~task_dispatcher()
{
    stop();
}

void task_dispatcher::run()
{
    threadpool_.init_thread_size(1);
    timer_.bind([this]{ check_request_timeout(); });
    timer_.start(1);
}

void task_dispatcher::add_result_handler(int serial_num, const result_handler& handler)
{
    lock_shared lock(mutex_);
    tasks_.emplace(serial_num, task{ handler, time(nullptr) });
}

void task_dispatcher::bind(const sub_handler& handler)
{
    sub_handler_ = handler;
}

void task_dispatcher::stop()
{
    timer_.destroy();
    threadpool_.stop();
    clear();
}

void task_dispatcher::clear()
{
    lock_shared lock(mutex_);
    tasks_.clear();
}

void task_dispatcher::dispatch(const std::shared_ptr<result>& ret)
{
    threadpool_.add_task(std::bind(&task_dispatcher::dispatch_thread, this, ret));
}

void task_dispatcher::dispatch_thread(const std::shared_ptr<result>& ret)
{
    if (ret->model == message_model::rpc)
    {
        deal_rpc_result(ret);
    }
    else
    {
        deal_sub_result(ret);
    }
}

void task_dispatcher::deal_rpc_result(const std::shared_ptr<result>& ret)
{
    task t;
    if (get_task(ret->serial_num, t))
    {
        remove_task(ret->serial_num);
        t.handler(ret);
    }
    else
    {
        log_warn << "Dispatch failed, serial num: " << ret->serial_num 
            << ", message name: " << ret->message->GetDescriptor()->full_name();
    }
}

void task_dispatcher::deal_sub_result(const std::shared_ptr<result>& ret)
{
    if (sub_handler_)
    {
        sub_handler_(ret);
    }
}

void task_dispatcher::check_request_timeout()
{
    lock_shared lock(mutex_);
    time_t current_time = time(nullptr);
    auto begin = tasks_.begin();
    auto end = tasks_.end();

    while (begin != end)
    {
        if (current_time - begin->second.begin_time >= request_timeout_)
        {
            log_warn << "Request timeout, serial_num: " << begin->first;
            begin = tasks_.erase(begin);
        }
        else
        {
            ++begin;
        }
    }
}

bool task_dispatcher::get_task(int serial_num, task& t)
{
    lock_shared lock(mutex_, true);
    auto iter = tasks_.find(serial_num);
    if (iter != tasks_.end())
    {
        t = iter->second;
        return true;
    }

    return false;
}

void task_dispatcher::remove_task(int serial_num)
{
    lock_shared lock(mutex_);
    tasks_.erase(serial_num);
}
