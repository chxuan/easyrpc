/* Copyright(C)
* For free
* All right reserved
* 
*/
/**
* @file CWorkerThread.h
* @brief 工作线程类
* @author highway-9, 787280310@qq.com
* @version 1.1.0
* @date 2016-02-17
*/

#ifndef _CWORKERTHREAD_H
#define _CWORKERTHREAD_H

#include "CThread.h"

class CWorkerThread : public CThread
{
public:
    CWorkerThread();
    virtual ~CWorkerThread();

    virtual void run();
};

#endif
