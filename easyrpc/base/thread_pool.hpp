#ifndef _THREADPOOL_H
#define _THREADPOOL_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <memory>
#include <functional>
#include <condition_variable>
#include <atomic>
#include <type_traits>

namespace easyrpc
{

static const std::size_t max_task_quque_size = 100000;
static const std::size_t max_thread_size = 30;

class thread_pool
{
public:
    using work_thread_ptr = std::shared_ptr<std::thread>;
    using task_t = std::function<void()>; 

    explicit thread_pool() : is_stop_threadpool_(false) {}

    ~thread_pool()
    {
        stop();
    }

    void init_thread_num(std::size_t num)
    {
        if (num <= 0 || num > max_thread_size)
        {
            std::string str = "Number of threads in the range of 1 to " + std::to_string(max_thread_size);
            throw std::invalid_argument(str);
        }

        for (std::size_t i = 0; i < num; ++i)
        {
            work_thread_ptr t = std::make_shared<std::thread>(std::bind(&thread_pool::run_task, this));
            thread_vec_.emplace_back(t);
        }
    }

    template<typename Function, typename... Args>
    void add_task(const Function& func, Args... args)
    {
        if (!is_stop_threadpool_)
        {
            task_t task = [&func, args...]{ return func(args...); };
            add_task_impl(task);
        }
    }

    template<typename Function, typename... Args>
    typename std::enable_if<std::is_class<Function>::value>::type add_task(Function& func, Args... args)
    {
        if (!is_stop_threadpool_)
        {
            task_t task = [&func, args...]{ return func(args...); };
            add_task_impl(task);
        }
    }

    template<typename Function, typename Self, typename... Args>
    void add_task(const Function& func, Self* self, Args... args)
    {
        if (!is_stop_threadpool_)
        {
            task_t task = [&func, &self, args...]{ return (*self.*func)(args...); };
            add_task_impl(task);
        }
    }

    void stop()
    {
        std::call_once(call_flag_, [this]{ terminate_all(); });
    }

private:
    void add_task_impl(const task_t& task)
    {
        {
            std::unique_lock<std::mutex> locker(task_queue_mutex_);
            while (task_queue_.size() == max_task_quque_size && !is_stop_threadpool_)
            {
                task_put_.wait(locker);
            }

            task_queue_.emplace(std::move(task));
        }

        task_get_.notify_one();
    }

    void terminate_all()
    {
        is_stop_threadpool_ = true;
        task_get_.notify_all();

        for (auto& iter : thread_vec_)
        {
            if (iter != nullptr)
            {
                if (iter->joinable())
                {
                    iter->join();
                }
            }
        }
        thread_vec_.clear();

        clean_task_queue();
    }

    void run_task()
    {
        while (true)
        {
            task_t task = nullptr;
            {
                std::unique_lock<std::mutex> locker(task_queue_mutex_);
                while (task_queue_.empty() && !is_stop_threadpool_)
                {
                    task_get_.wait(locker);
                }

                if (is_stop_threadpool_)
                {
                    break;
                }

                if (!task_queue_.empty())
                {
                    task = std::move(task_queue_.front());
                    task_queue_.pop();
                }
            }

            if (task != nullptr)
            {
                task();
                task_put_.notify_one();
            }
        }
    }

    void clean_task_queue()
    {
        std::lock_guard<std::mutex> locker(task_queue_mutex_);
        while (!task_queue_.empty())
        {
            task_queue_.pop();
        }
    }

private:
    std::vector<work_thread_ptr> thread_vec_;
    std::condition_variable task_put_;
    std::condition_variable task_get_;
    std::mutex task_queue_mutex_;
    std::queue<task_t> task_queue_;
    std::atomic<bool> is_stop_threadpool_;
    std::once_flag call_flag_;
};

}

#endif
