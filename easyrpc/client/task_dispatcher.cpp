#include "task_dispatcher.h"
#include "easyrpc/utility/logger.h"
#include "easyrpc/net/sig.h"
#include "easyrpc/client/result.h"

task_dispatcher::task_dispatcher() 
{
    qt_connect(complete_client_decode_data, std::bind(&task_dispatcher::deal_complete_client_decode_data, 
                                                      this, std::placeholders::_1));
}

task_dispatcher::~task_dispatcher()
{
    stop();
}

void task_dispatcher::run(time_t request_timeout)
{
    request_timeout_ = request_timeout;
    threadpool_.init_thread_size(1);
    timer_.bind([this]{ check_request_timeout(); });
    timer_.start(1);
}

void task_dispatcher::add_result_handler(int serial_num, const result_handler& handler)
{
    std::lock_guard<std::mutex> lock(mutex_);
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
    std::lock_guard<std::mutex> lock(mutex_);
    tasks_.clear();
}

void task_dispatcher::deal_complete_client_decode_data(const std::shared_ptr<result>& ret)
{
    threadpool_.add_task(std::bind(&task_dispatcher::dispatch_thread, this, ret));
}

void task_dispatcher::dispatch_thread(const std::shared_ptr<result>& ret)
{
    if (ret->serial_num() >= 0)
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
    if (get_task(ret->serial_num(), t))
    {
        remove_task(ret->serial_num());
        t.handler(ret);
    }
    else
    {
        log_warn << "Dispatch failed, serial num: " << ret->serial_num() 
            << ", message name: " << ret->message()->GetDescriptor()->full_name();
    }
}

void task_dispatcher::deal_sub_result(const std::shared_ptr<result>& ret)
{
    if (sub_handler_)
    {
        sub_handler_(ret->message());
    }
}

void task_dispatcher::check_request_timeout()
{
    std::lock_guard<std::mutex> lock(mutex_);
    time_t current_time = time(nullptr);
    auto begin = tasks_.begin();
    auto end = tasks_.end();

    while (begin != end)
    {
        if (current_time - begin->second.begin_time >= request_timeout_)
        {
            log_warn << "request timeout, serial_num: " << begin->first;
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
    std::lock_guard<std::mutex> lock(mutex_);
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
    std::lock_guard<std::mutex> lock(mutex_);
    tasks_.erase(serial_num);
}
