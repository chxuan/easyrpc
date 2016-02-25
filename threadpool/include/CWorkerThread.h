/* Copyright(C)
* For free
* All right reserved
* 
*/
/**
* @file CWorkerThread.h
* @brief 工作线程类
* @author highway-9, 787280310@qq.com
* @version 1.1.0
* @date 2016-02-17
*/

#ifndef _CWORKERTHREAD_H
#define _CWORKERTHREAD_H

#include "CThread.h"
#include <boost/atomic.hpp>

class CThreadPool;
class CJob;

typedef boost::shared_ptr<CThreadPool> CThreadPoolPtr;
typedef boost::shared_ptr<CJob> CJobPtr;

/**
* @brief 工作线程类，继承自CThread，执行具体的job
*/
class CWorkerThread
        : public CThread,
          public boost::enable_shared_from_this<CWorkerThread>
{
public:
    CWorkerThread();
    virtual ~CWorkerThread();

    /**
    * @brief run 实现run函数，在run函数里面等待并执行job
    */
    virtual void run();

public:
    /**
    * @brief setThreadPool 设置工作线程的线程池
    *
    * @param threadPool 线程池
    */
    void setThreadPool(CThreadPoolPtr threadPool);

private:
    CThreadPoolPtr m_threadPool;
    boost::condition_variable_any m_jobCond;
};

typedef boost::shared_ptr<CWorkerThread> CWorkerThreadPtr;

#endif
