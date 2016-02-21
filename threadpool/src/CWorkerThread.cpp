/* Copyright(C)
* For free
* All right reserved
* 
*/
/**
* @file CWorkerThread.cpp
* @brief 工作线程类
* @author highway-9, 787280310@qq.com
* @version 1.1.0
* @date 2016-02-17
*/

#include "CWorkerThread.h"
#include "CThreadPool.h"
#include "CJob.h"
#include <iostream>

CWorkerThread::CWorkerThread()
    : CThread(),
      m_jobData(NULL)
{
    m_threadPool.reset();
    m_job.reset();
}

CWorkerThread::~CWorkerThread()
{
    // Do nothing
}

void CWorkerThread::run()
{
    while (true)
    {
        {
            boost::unique_lock<boost::mutex> locker(m_jobMutex);
            while (m_job == NULL)
            {
                m_jobCond.wait(locker);
            }
        }

        m_job->run(m_jobData);
        m_job->setWorkThread(NULL);

        // job执行完后将job设置为NULL，job将自动析构
        m_job = NULL;

        m_threadPool->moveToIdleList(shared_from_this());

//        unsigned int idleNumOfThread = m_threadPool->idleNumOfThread();
//        unsigned int avalibleHighNumOfThread = m_threadPool->avalibleHighNumOfThread();
//        if (idleNumOfThread > avalibleHighNumOfThread)
//        {
//            unsigned int needDeleteNumOfThread = m_threadPool->idleNumOfThread()
//                    - m_threadPool->initNumOfThread();
//            //std::cout << "##########needDeleteNumOfThread: " << needDeleteNumOfThread << std::endl;
//            m_threadPool->deleteIdleThread(needDeleteNumOfThread);
//        }

        // 工作线程处理完job后，将workMutex解锁
        // 以便等待下一个job
        workMutex().unlock();
    }
}

CThreadPoolPtr CWorkerThread::threadPool() const
{
    return m_threadPool;
}

void CWorkerThread::setThreadPool(CThreadPoolPtr threadPool)
{
    assert(threadPool != NULL);
    boost::lock_guard<boost::mutex> locker(m_jobMutex);
    m_threadPool = threadPool;
}

CJobPtr CWorkerThread::job() const
{
    return m_job;
}

void CWorkerThread::setJob(CJobPtr job, void *jobData)
{
    assert(job != NULL);

    {
        boost::lock_guard<boost::mutex> locker(m_jobMutex);
        m_job = job;
        m_jobData = jobData;
        m_job->setWorkThread(shared_from_this());
    }

    m_jobCond.notify_one();
}

boost::mutex &CWorkerThread::workMutex()
{
    return m_workMutex;
}
