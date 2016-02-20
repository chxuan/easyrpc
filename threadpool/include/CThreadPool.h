/* Copyright(C)
* For free
* All right reserved
* 
*/
/**
* @file CThreadPool.h
* @brief 线程池类
* @author highway-9, 787280310@qq.com
* @version 1.1.0
* @date 2016-02-17
*/

#ifndef _CTHREADPOOL_H
#define _CTHREADPOOL_H

#include "CThread.h"
#include <vector>

class CWorkerThread;
class CJob;

class CThreadPool : public boost::enable_shared_from_this<CThreadPool>
{
public:
    typedef boost::shared_ptr<CWorkerThread> CWorkerThreadPtr;
    typedef boost::shared_ptr<CJob> CJobPtr;

    CThreadPool(unsigned int initNum);
    CThreadPool();
    ~CThreadPool();

public:
    void run(CJobPtr job, void* jobData);
    void terminateAll();

    void createIdleThread(unsigned int num);
    void deleteIdleThread(unsigned int num);

    CWorkerThreadPtr idleThread();

    void appendToIdleList(CWorkerThreadPtr workThread);
    void moveToBusyList(CWorkerThreadPtr idleThread);
    void moveToIdleList(CWorkerThreadPtr busyThread);

private:
    std::vector<CWorkerThreadPtr> m_threadList;
    std::vector<CWorkerThreadPtr> m_idleList;
    std::vector<CWorkerThreadPtr> m_busyList;

    boost::mutex m_idleMutex;
    boost::mutex m_busyMutex;
    boost::mutex m_jobMutex;

    boost::mutex m_idleCondMutex;
    boost::mutex m_busyCondMutex;

    boost::condition_variable_any m_idleCond;
    boost::condition_variable_any m_busyCond;

    unsigned int m_initNum;
};

typedef boost::shared_ptr<CThreadPool> CThreadPoolPtr;

#endif
