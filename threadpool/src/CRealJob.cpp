/* Copyright(C)
* For free
* All right reserved
* 
*/
/**
* @file CRealJob.cpp
* @brief 具体的任务类
* @author highway-9, 787280310@qq.com
* @version 1.1.0
* @date 2016-02-20
*/

#include "CRealJob.h"
#include <iostream>

CRealJob::CRealJob()
    : CJob(),
      m_doTask(NULL)
{
    // Do nothing
}

CRealJob::~CRealJob()
{
    // Do nothing
}

void CRealJob::run(void* jobData)
{
    if (m_doTask != NULL)
    {
        m_doTask(jobData);
    }
}

void CRealJob::setJob(OnDoTask func)
{
    assert(func != NULL);
    m_doTask = func;
}
