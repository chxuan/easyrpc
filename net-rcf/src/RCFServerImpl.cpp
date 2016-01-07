/* Copyright(C)
* For free
* All right reserved
* 
*/
/**
* @file RCFServerImpl.cpp
* @brief RCF服务端通信框架实现
* @author highway-9, 787280310@qq.com
* @version 1.1.0
* @date 2016-01-04
*/

#include "RCFServerImpl.h"
#include <assert.h>

#define MAX_THREAD_NUM      25      // 最大线程数量

RCFMessageImpl::RCFMessageImpl(const RCFServerImpl* impl)
{
    assert(impl != NULL);
    m_impl = impl;
}

RCFMessageImpl::~RCFMessageImpl()
{
    // Do nothing
}

void RCFMessageImpl::sendMessage(Message* message, Message* retMessage)
{
    Message* msg = new Message;
    msg->deserializeSelf(message);
        
    if (m_impl->m_messageCallback != NULL)
    {
        Message* retMsg = new Message;
        retMsg->m_messageType = msg->m_messageType;
        m_impl->m_messageCallback(msg, retMsg);
        if (retMsg != NULL)
        {
            _return = retMsg->serializeSelf();
            delete retMsg;
            retMsg = NULL;
        }
    }
    else
    {
        std::cout << "Message callback function is NULL" << std::endl;
    }

    if (msg != NULL)
    {
        delete msg;
        msg = NULL;
    }
}

RCFServerImpl::RCFServerImpl()
    : messageCallback(NULL),
    m_rcfInit(NULL),
    m_rcfServer(NULL),
    m_port(50001)
{
    // Do nothing
}

RCFServerImpl::~RCFServerImpl()
{
    stop();
    deinit();
}

void RCFServerImpl::init(unsigned int port)
{
    m_port = port;
}

bool RCFServerImpl::start()
{
    try
    {
        if (m_rcfInit == NULL)
        {
            m_rcfInit = boost::make_shared<RCF::RcfInitDeinit>();
        }

        if (m_rcfServer == NULL)
        {
            m_rcfServer = boost::make_shared<RCF::RcfServer>(RCF::TcpEndPoint(m_port));
            m_rcfServer->bind<I_RCFMessage>(m_rcfMessage);

            RCf::ThreadPoolPtr threadPool(new RCF::ThreadPool(1, MAX_THREAD_NUM));
            m_rcfServer->setThreadPool(threadPool);

            m_rcfServer->start();
        }
    }
    catch (const RCF::Exception& e)
    {
        std::cout << "Error: " << e.getErrorString() << std::endl;
        return false;
    }

    return true;
}

bool RCFServerImpl::stop()
{
    try
    {
        m_rcfServer->stop();    
    }
    catch (const RCF::Exception& e)
    {
        std::cout << "Error: " << e.getErrorString() << std::endl;
        return false;
    }

    return true;
}

void RCFServerImpl::deinit()
{
   // Do nothing 
}

void RCFServerImpl::setMessageCallback(MESSAGE_CALLBACK func)
{
    if (func != NULL)
    {
        m_messageCallback = func;
    }
}
