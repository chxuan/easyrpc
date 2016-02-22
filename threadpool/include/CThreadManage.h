/* Copyright(C)
* For free
* All right reserved
* 
*/
/**
* @file CThreadManage.h
* @brief 线程池管理类
* @author highway-9, 787280310@qq.com
* @version 1.1.0
* @date 2016-02-17
*/

#ifndef _CTHREADMANAGE_H
#define _CTHREADMANAGE_H

#include <boost/shared_ptr.hpp>

class CThreadPool;
class CJob;

/**
* @brief 线程池管理类，管理CThreadPool
*/
class CThreadManage
{
public:
    typedef boost::shared_ptr<CThreadPool> CThreadPoolPtr;
    typedef boost::shared_ptr<CJob> CJobPtr;

    CThreadManage();
    ~CThreadManage();

public:
    void initThreadNum(unsigned int initNumOfThread);
    void run(CJobPtr job, void* jobData);
    void terminateAll();

private:
    CThreadPoolPtr m_threadPool;
};

typedef boost::shared_ptr<CThreadManage> CThreadManagePtr;

#endif
