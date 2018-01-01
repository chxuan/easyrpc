/**
 * @file lock_shared.h
 * @brief 封装读写锁
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2018-01-01
 */
#pragma once

#include <pthread.h>

struct shared_mutex
{
    shared_mutex()
    {
        pthread_rwlock_init(&mt, nullptr);
    }

    ~shared_mutex()
    {
        pthread_rwlock_destroy(&mt);
    }

    pthread_rwlock_t mt;
};

class lock_shared
{
public:
    lock_shared(shared_mutex& mt, bool shared = false)
        : mt_(mt)
    { 
        if (shared)
        {
            pthread_rwlock_rdlock(&mt_.mt);
        }
        else
        {
            pthread_rwlock_wrlock(&mt_.mt);
        }
    }

    ~lock_shared()
    {
        pthread_rwlock_unlock(&mt_.mt);
    }

private:
    shared_mutex& mt_;
};
