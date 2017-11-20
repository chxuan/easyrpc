/**
 * @file threadsafe_list.hpp
 * @brief 线程安全的队列
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-05
 */
#pragma once

#include <list>
#include <mutex>

template<typename T>
class threadsafe_list
{
public:
    void emplace_back(const T& t)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        list_.emplace_back(t);
    }

    T front()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return list_.front();
    }

    void pop_front()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        list_.pop_front();
    }

    void clear()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        list_.clear();
    }

    bool empty()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return list_.empty();
    }

    std::size_t size()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return list_.size();
    }

private:
    std::list<T> list_;
    std::mutex mutex_;
};
