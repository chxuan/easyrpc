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
#include <boost/function.hpp>

class Message;
typedef boost::shared_ptr<Message> MessagePtr;
typedef boost::function1<void, MessagePtr> OnReciveMessage;

/**
* @brief 任务类，继承自CJob
*/
class CRealJob : public CJob
{
public:
    CRealJob(OnReciveMessage func, MessagePtr message);
    virtual ~CRealJob();

    /**
    * @brief run 执行任务
    */
    virtual void run();

private:
    OnReciveMessage m_onReciveMessage;
    MessagePtr m_message;
};

typedef boost::shared_ptr<CRealJob> CRealJobPtr;

#endif
