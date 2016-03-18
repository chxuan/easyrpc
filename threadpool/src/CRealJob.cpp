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
#include <assert.h>

CRealJob::CRealJob(OnDoTask func, void* jobData)
    : CJob(),
      m_jobData(jobData)
{
    assert(func != NULL);
    m_doTask = func;
}

CRealJob::~CRealJob()
{
    // Do nothing
}

void CRealJob::run()
{
    if (m_doTask != NULL)
    {
        m_doTask(m_jobData);
    }
}
