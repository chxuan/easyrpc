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

    /**
    * @brief run 执行任务
    *
    * @param jobData 任务数据
    */
    virtual void run(void* jobData);

    /**
    * @brief setJob 设置任务，该任务为回调函数
    *
    * @param func 回调函数
    */
    virtual void setJob(OnDoTask func);

private:
    OnDoTask m_doTask;
};

typedef boost::shared_ptr<CRealJob> CRealJobPtr;

#endif
