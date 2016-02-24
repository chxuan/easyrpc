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
    : CThread(),
      m_jobData(NULL),
      m_isSetJob(false)
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
            boost::unique_lock<boost::mutex> locker(m_threadPool->jobQueueGetMutex());
            while (m_threadPool->isJobQueueEmpty())
            {
                m_threadPool->jobQueueGetCond().wait(locker);
            }
        }
std::cout << __LINE__ << std::endl;
        m_threadPool->moveToBusyList(shared_from_this());
std::cout << __LINE__ << std::endl;
        CJobPtr job;
std::cout << __LINE__ << std::endl;
        m_threadPool->getJobFromJobQueue(job);
std::cout << __LINE__ << std::endl;
        m_threadPool->jobQueuePutCond().notify_one();
std::cout << __LINE__ << std::endl;
        job->run(NULL);
std::cout << __LINE__ << std::endl;
        job.reset();
std::cout << __LINE__ << std::endl;
        m_threadPool->moveToIdleList(shared_from_this());
#if 0
        {
            boost::unique_lock<boost::mutex> locker(m_jobMutex);
            while (!m_isSetJob)
            {
                m_jobCond.wait(locker);
            }
        }

        m_job->run(m_jobData);
        m_job.reset();
        m_threadPool->moveToIdleList(shared_from_this());
        m_isSetJob = false;

        // 工作线程处理完job后，将workMutex解锁
        // 以便等待下一个job
        workMutex().unlock();
#endif
    }
}

void CWorkerThread::setThreadPool(CThreadPoolPtr threadPool)
{
    assert(threadPool != NULL);
    boost::lock_guard<boost::mutex> locker(m_jobMutex);
    m_threadPool = threadPool;
}

void CWorkerThread::setJob(CJobPtr job, void *jobData)
{
    assert(job != NULL);

    {
        boost::lock_guard<boost::mutex> locker(m_jobMutex);
        m_job = job;
        m_jobData = jobData;
        m_job->setWorkThread(shared_from_this());
        m_isSetJob = true;
    }

    m_jobCond.notify_one();
}

boost::mutex &CWorkerThread::workMutex()
{
    return m_workMutex;
}
