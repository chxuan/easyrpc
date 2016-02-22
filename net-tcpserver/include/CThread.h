/* Copyright(C)
* For free
* All right reserved
* 
*/
/**
* @file CThread.h
* @brief 线程父类
* @author highway-9, 787280310@qq.com
* @version 1.1.0
* @date 2016-02-17
*/

#ifndef _CTHREAD_H
#define _CTHREAD_H

#include <assert.h>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>

/**
* @brief 线程类，封装了boost.thread的常用函数
*/
class CThread
{
public:
    typedef boost::shared_ptr<boost::thread> ThreadPtr;

    CThread()
    {
        m_thread.reset();
    }

    virtual ~CThread()
    {
        if (m_thread != NULL)
        {
            if (m_thread->joinable())
            {
                m_thread->join();
            }
        }
    }

    /**
    * @brief run 继承自CThread的类都要实现run函数
    */
    virtual void run() = 0;

public:
    /**
    * @brief start 启动线程是调用handleThread，handleThread再调用run函数
    */
    void start()
    {
        if (m_thread == NULL)
        {
            m_thread = boost::make_shared<boost::thread>(boost::bind(&CThread::handleThread, this));
        }
    }

    bool joinable() const
    {
        assert(m_thread != NULL);
        return m_thread->joinable();
    }

    void join()
    {
        assert(m_thread != NULL);
        m_thread->join();
    }

    void detach()
    {
        assert(m_thread != NULL);
        m_thread->detach();
    }

    /**
    * @brief interrupt 强制中断线程的执行
    */
    void interrupt()
    {
        assert(m_thread != NULL);
        m_thread->interrupt();
    }

    bool interruptionRequested() const
    {
        assert(m_thread != NULL);
        m_thread->interruption_requested();
    }

    boost::thread::id threadID() const
    {
        assert(m_thread != NULL);
        return m_thread->get_id();
    }

private:
    void handleThread()
    {
        run();
    }

private:
    ThreadPtr m_thread;
};

typedef boost::shared_ptr<CThread> CThreadPtr;

#endif
