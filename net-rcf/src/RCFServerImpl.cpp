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

#define MAX_THREAD_NUM      25      // 最大线程数量

RCFServerImpl::RCFServerImpl()
    : m_rcfInit(NULL),
    m_rcfServer(NULL),
    m_rcfMessageHandler(NULL),
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
    if (m_rcfMessageHandler == NULL)
    {
        std::cout << "RCF message hander is NULL" << std::endl;
        return false;
    }

    try
    {
        if (m_rcfInit == NULL)
        {
            m_rcfInit = boost::make_shared<RCF::RcfInitDeinit>();
        }

        if (m_rcfServer == NULL)
        {
            m_rcfServer = boost::make_shared<RCF::RcfServer>(RCF::TcpEndPoint(m_port));

            m_rcfServer->bind<RCFMessageHandler>(*m_rcfMessageHandler);

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

void RCFServerImpl::setMessageHandler(RCFMessageHandler rcfMessageHandler)
{
    if (rcfMessageHandler != NULL)
    {
        m_rcfMessageHandler = rcfMessageHandler;
    }
}
