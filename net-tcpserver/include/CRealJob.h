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
#include <functional>

class Message;
typedef std::shared_ptr<Message> MessagePtr;
typedef std::function<void (MessagePtr, const std::string&)> OnReciveMessage;

/**
* @brief 任务类，继承自CJob
*/
class CRealJob : public CJob
{
public:
    CRealJob(OnReciveMessage func, MessagePtr message, const std::string& remoteAddress);
    virtual ~CRealJob();

    /**
    * @brief run 执行任务
    */
    virtual void run();

private:
    OnReciveMessage m_onReciveMessage;
    MessagePtr m_message;
    std::string m_remoteAddress;
};

typedef std::shared_ptr<CRealJob> CRealJobPtr;

#endif
