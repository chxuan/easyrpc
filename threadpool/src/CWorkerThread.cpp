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

CWorkerThread::CWorkerThread()
    : CThread()
{
    m_threadPool.reset();
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
            boost::unique_lock<boost::mutex> locker(m_threadPool->m_jobQueueMutex);
            while (m_threadPool->m_jobQueue.empty())
            {
                m_threadPool->m_jobQueueGetCond.wait(locker);
            }
        }

        //m_threadPool->moveToBusyList(shared_from_this());

        CJobPtr job;
        {
            boost::lock_guard<boost::mutex> locker(m_threadPool->m_jobQueueMutex);
            if (!m_threadPool->m_jobQueue.empty())
            {
                job = m_threadPool->m_jobQueue.front();
                m_threadPool->m_jobQueue.pop();
            }
        }

        if (job.use_count() != 0)
        {
            //m_threadPool->m_jobQueuePutCond.notify_one();
            job->run(NULL);
            job.reset();
        }
        else
        {
            std::cout << __LINE__ << std::endl;
        }

        //m_threadPool->moveToIdleList(shared_from_this());

        //m_threadPool->dynamicAdjustThreadPoolSize();
    }
}

void CWorkerThread::setThreadPool(CThreadPoolPtr threadPool)
{
    assert(threadPool != NULL);
    m_threadPool = threadPool;
}
