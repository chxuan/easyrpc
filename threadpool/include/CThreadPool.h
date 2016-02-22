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

typedef boost::shared_ptr<CWorkerThread> CWorkerThreadPtr;
typedef boost::shared_ptr<CJob> CJobPtr;

/**
* @brief 线程池类，管理并发线程
*/
class CThreadPool : public boost::enable_shared_from_this<CThreadPool>
{
public:
    friend class CWorkerThread;

    CThreadPool();
    ~CThreadPool();

public:
    void initThreadNum(unsigned int initNumOfThread);
    void run(CJobPtr job, void* jobData);
    void terminateAll();

    unsigned int maxNumOfThread() const;
    unsigned int avalibleLowNumOfThread() const;
    unsigned int avalibleHighNumOfThread() const;
    unsigned int initNumOfThread() const;
    unsigned int allNumOfThread() const;
    unsigned int busyNumOfThread() const;
    unsigned int idleNumOfThread() const;

private:
    void createIdleThread(unsigned int num);
    void deleteIdleThread(unsigned int num);
    CWorkerThreadPtr idleThread();
    void appendToIdleList(CWorkerThreadPtr workThread);
    void moveToBusyList(CWorkerThreadPtr idleThread);
    void moveToIdleList(CWorkerThreadPtr busyThread);
    void dynamicAdjustThreadPoolSize();

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
    boost::condition_variable_any m_maxCond;

    unsigned int m_maxNumOfThread;              // the max thread num that can create at the same time
    unsigned int m_avalibleLowNumOfThread;      // The min num of idle thread that shoule kept
    unsigned int m_avalibleHighNumOfThread;     // The max num of idle thread that kept at the same time
    unsigned int m_initNumOfThread;             // Normal thread num
};

typedef boost::shared_ptr<CThreadPool> CThreadPoolPtr;

#endif
