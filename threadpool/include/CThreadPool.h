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
#include <queue>

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
    /**
    * @brief initThreadNum 初始化线程数量
    *
    * @param initNumOfThread 线程数量
    */
    void initThreadNum(unsigned int initNumOfThread);

    /**
    * @brief run 执行具体的job
    *
    * @param job 具体的任务
    * @param jobData 任务数据
    */
    void run(CJobPtr job, void* jobData);

    /**
    * @brief terminateAll 终止全部的线程，即使该线程在处理任务或等待任务
    */
    void terminateAll();

private:
    /**
    * @brief createIdleThread 创建空闲线程
    *
    * @param num 线程数量
    */
    void createIdleThread(unsigned int num);

    /**
    * @brief deleteIdleThread 删除空闲线程
    *
    * @param num 线程数量
    */
    void deleteIdleThread(unsigned int num);

    /**
    * @brief appendToIdleList 将创建好的空闲线程放入空闲线程列表
    *
    * @param workThread 工作线程
    */
    void appendToIdleList(CWorkerThreadPtr workThread);

    /**
    * @brief moveToBusyList 将空闲线程移动到繁忙线程列表
    *
    * @param idleThread 空闲线程
    */
    void moveToBusyList(CWorkerThreadPtr idleThread);

    /**
    * @brief moveToIdleList 将繁忙线程移动到空闲线程列表
    *
    * @param busyThread 繁忙线程
    */
    void moveToIdleList(CWorkerThreadPtr busyThread);

    /**
    * @brief dynamicAdjustThreadPoolSize 通过增、减空闲线程来动态改变线程池的大小
    */
    void dynamicAdjustThreadPoolSize();

    /**
    * @brief cleanJobQueue 清除任务队列
    */
    void cleanJobQueue();

private:
    std::vector<CWorkerThreadPtr> m_threadList;
    std::vector<CWorkerThreadPtr> m_idleList;
    std::vector<CWorkerThreadPtr> m_busyList;

    boost::mutex m_idleListMutex;
    boost::mutex m_busyListMutex;

    boost::condition_variable_any m_jobQueuePutCond;
    boost::condition_variable_any m_jobQueueGetCond;
    boost::mutex m_jobQueueMutex;
    std::queue<CJobPtr> m_jobQueue;

    unsigned int m_maxNumOfThread;              ///< the max thread num that can create at the same time
    unsigned int m_avalibleLowNumOfThread;      ///< The min num of idle thread that shoule kept
    unsigned int m_avalibleHighNumOfThread;     ///< The max num of idle thread that kept at the same time
    unsigned int m_initNumOfThread;             ///< Normal thread num
};

typedef boost::shared_ptr<CThreadPool> CThreadPoolPtr;

#endif
