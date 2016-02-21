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

static const unsigned int MaxNumOfThread = 30;

CThreadPool::CThreadPool()
    : m_maxNumOfThread(MaxNumOfThread),
      m_avalibleLowNumOfThread(3),
      m_avalibleHighNumOfThread(20),
      m_initNumOfThread(0)
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
    std::cout << "m_avalibleLowNumOfThread: " << m_avalibleLowNumOfThread << std::endl;
    m_avalibleHighNumOfThread = m_initNumOfThread + 10;
    createIdleThread(m_initNumOfThread);
}

void CThreadPool::run(CJobPtr job, void *jobData)
{
    assert(job != NULL);

    {
        boost::unique_lock<boost::mutex> locker(m_busyMutex);
        std::cout << "################busyNumOfThread: " << busyNumOfThread() << std::endl;
        while (busyNumOfThread() == maxNumOfThread())
        {
            std::cout << "busy##################################busy" << std::endl;
            m_maxCond.wait(locker);
        }
    }

#if 1
    if (idleNumOfThread() > avalibleHighNumOfThread())
    {
        unsigned int needDeleteNumOfThread = idleNumOfThread() - initNumOfThread();
        std::cout << "##########needDeleteNumOfThread: " << needDeleteNumOfThread << std::endl;
        deleteIdleThread(needDeleteNumOfThread);
    }
#endif

    std::cout << "idleNumOfThread: " << idleNumOfThread() << std::endl;
    if (idleNumOfThread() < avalibleLowNumOfThread())
    {
        if (allNumOfThread() + initNumOfThread() - idleNumOfThread() < maxNumOfThread())
        {
            unsigned int needCreateNumOfThread = initNumOfThread() - idleNumOfThread();
            std::cout << "####Create thread num: " << needCreateNumOfThread << std::endl;
            createIdleThread(needCreateNumOfThread);
        }
        else
        {
            unsigned int needCreateNumOfThread = maxNumOfThread() - allNumOfThread();
            std::cout << "####Create thread num: " << needCreateNumOfThread << std::endl;
            createIdleThread(needCreateNumOfThread);
        }
    }

    CWorkerThreadPtr workThread = idleThread();
    if (workThread != NULL)
    {
        // 在执行工作线程之前将workMutex上锁
        // 防止在执行工作线程时，对工作线程setJob
        workThread->workMutex().lock();

        moveToBusyList(workThread);
        workThread->setThreadPool(shared_from_this());
        job->setWorkThread(workThread);
        workThread->setJob(job, jobData);
    }
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
}

unsigned int CThreadPool::maxNumOfThread() const
{
    return m_maxNumOfThread;
}

unsigned int CThreadPool::avalibleLowNumOfThread() const
{
    return m_avalibleLowNumOfThread;
}

unsigned int CThreadPool::avalibleHighNumOfThread() const
{
    return m_avalibleHighNumOfThread;
}

unsigned int CThreadPool::initNumOfThread() const
{
    return m_initNumOfThread;
}

unsigned int CThreadPool::allNumOfThread() const
{
    return m_threadList.size();
}

unsigned int CThreadPool::busyNumOfThread() const
{
    return m_busyList.size();
}

unsigned int CThreadPool::idleNumOfThread() const
{
    return m_idleList.size();
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
    boost::lock_guard<boost::mutex> locker(m_idleMutex);
    if (num > idleNumOfThread())
    {
        num = idleNumOfThread();
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

CWorkerThreadPtr CThreadPool::idleThread()
{
    boost::unique_lock<boost::mutex> locker(m_idleMutex);
    while (idleNumOfThread() == 0)
    {
        m_idleCond.wait(locker);
    }

    if (idleNumOfThread() > 0)
    {
        return m_idleList.front();
    }

    return CWorkerThreadPtr();
}

void CThreadPool::appendToIdleList(CWorkerThreadPtr workThread)
{
    boost::lock_guard<boost::mutex> locker(m_idleMutex);
    m_idleList.push_back(workThread);
    m_threadList.push_back(workThread);
}

void CThreadPool::moveToBusyList(CWorkerThreadPtr idleThread)
{
    {
        boost::lock_guard<boost::mutex> locker(m_busyMutex);
        m_busyList.push_back(idleThread);
    }

    boost::lock_guard<boost::mutex> locker(m_idleMutex);
    auto iter = std::find(m_idleList.begin(), m_idleList.end(), idleThread);
    if (iter != m_idleList.end())
    {
        m_idleList.erase(iter);
    }
}

void CThreadPool::moveToIdleList(CWorkerThreadPtr busyThread)
{
    {
        boost::lock_guard<boost::mutex> locker(m_idleMutex);
        m_idleList.push_back(busyThread);
    }

    {
        boost::lock_guard<boost::mutex> locker(m_busyMutex);
        auto iter = std::find(m_busyList.begin(), m_busyList.end(), busyThread);
        if (iter != m_busyList.end())
        {
            m_busyList.erase(iter);
        }
    }

    m_idleCond.notify_one();
    m_maxCond.notify_one();
}
