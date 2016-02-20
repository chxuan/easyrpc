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

CThreadPool::CThreadPool()
    : m_initNum(0)
{
    // Do nothing
}

CThreadPool::~CThreadPool()
{
    terminateAll();
}

void CThreadPool::initThreadNum(unsigned int initNum)
{
    assert(initNum > 0 && initNum <= 30);
    m_initNum = initNum;
    createIdleThread(m_initNum);
}

void CThreadPool::run(CJobPtr job, void *jobData)
{
    assert(job != NULL);
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
    if (num > m_idleList.size())
    {
        num = m_idleList.size();
    }

    for (unsigned int i = 0; i < num; ++i)
    {
        CWorkerThreadPtr idleThread = m_idleList.front();
        auto iter = std::find(m_idleList.begin(), m_idleList.end(), idleThread);
        if (iter != m_idleList.end())
        {
            m_idleList.erase(iter);
        }
    }
}

CWorkerThreadPtr CThreadPool::idleThread()
{
    boost::unique_lock<boost::mutex> locker(m_idleMutex);
    while (m_idleList.size() == 0)
    {
        m_idleCond.wait(locker);
    }

    if (m_idleList.size() > 0)
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

    boost::lock_guard<boost::mutex> locker(m_busyMutex);
    auto iter = std::find(m_busyList.begin(), m_busyList.end(), busyThread);
    if (iter != m_busyList.end())
    {
        m_busyList.erase(iter);
    }

    m_idleCond.notify_one();
}
