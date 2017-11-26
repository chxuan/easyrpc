#include "thread_pool.h"

thread_pool::~thread_pool()
{
    stop();
}

void thread_pool::init_thread_size(int size)
{
    for (int i = 0; i < size; ++i)
    {
        auto t = std::make_shared<std::thread>(std::bind(&thread_pool::run_task, this));
        threads_.emplace_back(t);
    }
}

void thread_pool::stop()
{
    std::call_once(call_flag_, [this]{ stop_impl(); });
}

std::size_t thread_pool::size()
{
    std::unique_lock<std::mutex> lock(mutex_);
    return task_queue_.size();
}

void thread_pool::run_task()
{
    while (true)
    {
        std::function<void()> task;

        {
            std::unique_lock<std::mutex> lock(mutex_);
            cond_.wait(lock, [this]{ return threadpool_stoped_ || !task_queue_.empty(); });
            if (threadpool_stoped_ && task_queue_.empty())
            {
                return;
            }
            task = std::move(task_queue_.front());
            task_queue_.pop();
        }

        task();
    }
}

void thread_pool::stop_impl()
{
    threadpool_stoped_ = true;
    cond_.notify_all();
    for(auto& t: threads_)
    {
        if (t->joinable())
        {
            t->join();
        }
    }
}
