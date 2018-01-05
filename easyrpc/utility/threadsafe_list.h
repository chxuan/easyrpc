/**
 * @file threadsafe_list.hpp
 * @brief 线程安全的队列
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-05
 */
#pragma once

#include <list>
#include "shared_mutex.h"

template<typename T>
class threadsafe_list
{
public:
    void emplace_back(const T& t)
    {
        lock_shared lock(mutex_);
        list_.emplace_back(t);
    }

    T front()
    {
        lock_shared lock(mutex_, true);
        return list_.front();
    }

    void pop_front()
    {
        lock_shared lock(mutex_);
        list_.pop_front();
    }

    void clear()
    {
        lock_shared lock(mutex_);
        list_.clear();
    }

    bool empty()
    {
        lock_shared lock(mutex_, true);
        return list_.empty();
    }

    std::size_t size()
    {
        lock_shared lock(mutex_, true);
        return list_.size();
    }

private:
    std::list<T> list_;
    shared_mutex mutex_;
};
