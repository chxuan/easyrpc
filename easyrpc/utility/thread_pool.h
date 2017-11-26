/**
 * @file thread_pool.h
 * @brief 线程池
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-05
 */
#pragma once

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>

class thread_pool 
{
public:
    ~thread_pool();
    void init_thread_size(int size);
    void stop();
    std::size_t size();

    template<typename Function, typename... Args>
    auto add_task(Function&& func, Args&&... args) -> std::future<typename std::result_of<Function(Args...)>::type>
    {
        if (threadpool_stoped_)
        {
            throw std::runtime_error("Add task on stopped thread pool");
        }

        using return_type = typename std::result_of<Function(Args...)>::type;
        auto task = std::make_shared<std::packaged_task<return_type()>>( std::bind(std::forward<Function>(func), std::forward<Args>(args)...));
        std::future<return_type> res = task->get_future();

        std::unique_lock<std::mutex> lock(mutex_);
        task_queue_.emplace([task](){ (*task)(); });
        lock.unlock();

        cond_.notify_one();
        return res;
    }

private:
    void run_task();
    void stop_impl();

private:
    std::vector<std::shared_ptr<std::thread>> threads_;
    std::queue<std::function<void()>> task_queue_;

    std::mutex mutex_;
    std::condition_variable cond_;
    std::atomic<bool> threadpool_stoped_{ false };
    std::once_flag call_flag_;
};
