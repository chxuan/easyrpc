/* Copyright(C)
* For free
* All right reserved
* 
*/
/**
* @file CRealJob.h
* @brief 具体的任务类
* @author highway-9, 787280310@qq.com
* @version 1.1.0
* @date 2016-02-20
*/

#ifndef _CREALJOB_H
#define _CREALJOB_H

#include "CJob.h"

/**
* @brief 任务类，继承自CJob
*/
class CRealJob : public CJob
{
public:
    CRealJob();
    virtual ~CRealJob();

    virtual void run(void* jobData);
    virtual void setJob(OnDoTask func);

private:
    OnDoTask m_doTask;
};

typedef boost::shared_ptr<CRealJob> CRealJobPtr;

#endif
