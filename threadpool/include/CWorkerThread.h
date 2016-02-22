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

    /**
    * @brief setJob 设置工作线程的任务
    *
    * @param job 任务
    * @param jobData 任务数据
    */
    void setJob(CJobPtr job, void* jobData);

    /**
    * @brief workMutex 获得工作锁，该锁在工作线程开始执行job时上锁
    * 在结束job时解锁
    *
    * @return 工作锁
    */
    boost::mutex& workMutex();

private:
    CThreadPoolPtr m_threadPool;
    CJobPtr m_job;
    void* m_jobData;

    boost::mutex m_workMutex;
    boost::mutex m_jobMutex;
    boost::condition_variable_any m_jobCond;

    boost::atomic<bool> m_isSetJob;
};

typedef boost::shared_ptr<CWorkerThread> CWorkerThreadPtr;

#endif
