/* Copyright(C)
* For free
* All right reserved
* 
*/
/**
* @file CJob.h
* @brief 任务父类
* @author highway-9, 787280310@qq.com
* @version 1.1.0
* @date 2016-02-20
*/

#ifndef _CJOB_H
#define _CJOB_H

#include "CThread.h"
#include <string>

typedef boost::function1<void, void*> OnDoTask;

class CJob
{
public:
    CJob()
        : m_jobNo(0)
    {
        m_workThread.reset();
    }

    virtual ~CJob()
    {
        // Do nothing
    }

    virtual void run(void* jobData) = 0;
    virtual void setJob(OnDoTask func) = 0;

public:
    int jobNo() const { return m_jobNo; }
    void setJobNo(int jobNo) { m_jobNo = jobNo; }

    std::string jobName() const { return m_jobName; }
    void setJobName(const std::string& jobName) { m_jobName = jobName; }

    CThreadPtr workThread() const { return m_workThread; }
    void setWorkThread(CThreadPtr workThread) { m_workThread = workThread; }

private:
    int m_jobNo;
    std::string m_jobName;
    CThreadPtr m_workThread;
};

typedef boost::shared_ptr<CJob> CJobPtr;

#endif
