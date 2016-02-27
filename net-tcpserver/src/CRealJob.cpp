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

CRealJob::CRealJob(OnReciveMessage func, MessagePtr message)
    : CJob(),
      m_message(message)
{
    assert(func != NULL);
    m_onReciveMessage = func;
}

CRealJob::~CRealJob()
{
    // Do nothing
}

void CRealJob::run()
{
    if (m_onReciveMessage != NULL)
    {
        m_onReciveMessage(m_message);
    }
}
