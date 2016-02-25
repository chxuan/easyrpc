/* Copyright(C)
* For free
* All right reserved
* 
*/
/**
* @file CThreadPool.cpp
* @brief 线程池类
* @author highway-9, 787280310@qq.com
* @version 1.1.0
* @date 2016-02-17
*/

#include "CThreadPool.h"
#include "CWorkerThread.h"
#include "CJob.h"

static const unsigned int MaxJobQueueSize = 100000;
static const unsigned int MaxNumOfThread = 30;
static const unsigned int AvalibleLowNumOfThread = 3;
static const unsigned int AvalibleHighNumOfThread = 20;
static const unsigned int InitNumOfThread = 10;

CThreadPool::CThreadPool()
    : m_maxNumOfThread(MaxNumOfThread),
      m_avalibleLowNumOfThread(AvalibleLowNumOfThread),
      m_avalibleHighNumOfThread(AvalibleHighNumOfThread),
      m_initNumOfThread(InitNumOfThread)
{
    // Do nothing
}

CThreadPool::~CThreadPool()
{
    terminateAll();
}

void CThreadPool::initThreadNum(unsigned int initNumOfThread)
{
    assert(initNumOfThread > 0 && initNumOfThread <= MaxNumOfThread);
    m_initNumOfThread = initNumOfThread;
    m_maxNumOfThread = MaxNumOfThread;
    m_avalibleLowNumOfThread = m_initNumOfThread - 10 > 0 ? m_initNumOfThread - 10 : 3;
    m_avalibleHighNumOfThread = m_initNumOfThread + 10;
    createIdleThread(m_initNumOfThread);
}

void CThreadPool::run(CJobPtr job, void *jobData)
{
    assert(job != NULL);

    {
        boost::unique_lock<boost::mutex> locker(m_jobQueueMutex);
        while (m_jobQueue.size() == MaxJobQueueSize)
        {
            m_jobQueuePutCond.wait(locker);
        }
    }

    {
        boost::lock_guard<boost::mutex> locker(m_jobQueueMutex);
        m_jobQueue.push(job);
    }
    m_jobQueueGetCond.notify_one();
}

void CThreadPool::terminateAll()
{
    for (auto& iter : m_threadList)
    {
        // 中断工作线程，即使工作线程在处理job或在等待job
        iter->interrupt();
        if (iter->joinable())
        {
            iter->join();
        }
    }

    m_threadList.clear();
    m_idleList.clear();
    m_busyList.clear();

    cleanJobQueue();
}

void CThreadPool::createIdleThread(unsigned int num)
{
    for (unsigned int i = 0; i < num; ++i)
    {
        CWorkerThreadPtr idleThread(new CWorkerThread);
        idleThread->setThreadPool(shared_from_this());
        appendToIdleList(idleThread);
        idleThread->start();
    }
}

void CThreadPool::deleteIdleThread(unsigned int num)
{
    boost::lock_guard<boost::mutex> locker(m_idleListMutex);
    if (num > m_idleList.size())
    {
        num = m_idleList.size();
    }

    for (unsigned int i = 0; i < num; ++i)
    {
        CWorkerThreadPtr idleThread = m_idleList.front();

        // 中断工作线程
        idleThread->interrupt();
        if (idleThread->joinable())
        {
            idleThread->join();
        }

        auto iter = std::find(m_idleList.begin(), m_idleList.end(), idleThread);
        if (iter != m_idleList.end())
        {
            m_idleList.erase(iter);
        }

        iter = std::find(m_threadList.begin(), m_threadList.end(), idleThread);
        if (iter != m_threadList.end())
        {
            m_threadList.erase(iter);
        }
    }
}

void CThreadPool::appendToIdleList(CWorkerThreadPtr workThread)
{
    boost::lock_guard<boost::mutex> locker(m_idleListMutex);
    m_idleList.push_back(workThread);
    m_threadList.push_back(workThread);
}

void CThreadPool::moveToBusyList(CWorkerThreadPtr idleThread)
{
    {
        boost::lock_guard<boost::mutex> locker(m_busyListMutex);
        m_busyList.push_back(idleThread);
    }

    boost::lock_guard<boost::mutex> locker(m_idleListMutex);
    auto iter = std::find(m_idleList.begin(), m_idleList.end(), idleThread);
    if (iter != m_idleList.end())
    {
        m_idleList.erase(iter);
    }
}

void CThreadPool::moveToIdleList(CWorkerThreadPtr busyThread)
{
    {
        boost::lock_guard<boost::mutex> locker(m_idleListMutex);
        m_idleList.push_back(busyThread);
    }

    boost::lock_guard<boost::mutex> locker(m_busyListMutex);
    auto iter = std::find(m_busyList.begin(), m_busyList.end(), busyThread);
    if (iter != m_busyList.end())
    {
        m_busyList.erase(iter);
    }
}

void CThreadPool::dynamicAdjustThreadPoolSize()
{
    if (m_idleList.size() > m_avalibleHighNumOfThread)
    {
        unsigned int needDeleteNumOfThread = m_idleList.size() - m_initNumOfThread;
        deleteIdleThread(needDeleteNumOfThread);
    }

    if (m_idleList.size() < m_avalibleLowNumOfThread)
    {
        std::cout << "******************" << std::endl;
        if (m_threadList.size() + m_initNumOfThread - m_idleList.size() < m_maxNumOfThread)
        {
            unsigned int needCreateNumOfThread = m_initNumOfThread - m_idleList.size();
            std::cout << "###########needCreateNumOfThread: " << needCreateNumOfThread << std::endl;
            createIdleThread(needCreateNumOfThread);
        }
        else
        {
            unsigned int needCreateNumOfThread = m_maxNumOfThread - m_threadList.size();
            std::cout << "###########needCreateNumOfThread: " << needCreateNumOfThread << std::endl;
            createIdleThread(needCreateNumOfThread);
        }
    }
}

void CThreadPool::cleanJobQueue()
{
    boost::lock_guard<boost::mutex> locker(m_jobQueueMutex);
    while (!m_jobQueue.empty())
    {
        m_jobQueue.pop();
    }
}
