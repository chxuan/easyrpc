/* Copyright(C)
* For free
* All right reserved
* 
*/
/**
* @file CXJob.h
* @brief 具体的任务类
* @author highway-9, 787280310@qq.com
* @version 1.1.0
* @date 2016-02-20
*/

#ifndef _CXJOB_H
#define _CXJOB_H

#include "CJob.h"

class CXJob : public CJob
{
public:
    CXJob();
    virtual ~CXJob();

    virtual void run(void* jobData);
};

typedef boost::shared_ptr<CXJob> CXJobPtr;

#endif
