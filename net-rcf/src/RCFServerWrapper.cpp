/* Copyright(C)
* For free
* All right reserved
* 
*/
/**
* @file RCFServerWrapper.cpp
* @brief RCF服务端通信框架包装实现
* @author highway-9, 787280310@qq.com
* @version 1.1.0
* @date 2016-01-04
*/

#include "RCFServerWrapper.h"
#include "RCFServerImpl.h"

RCFServerWrapper::RCFServerWrapper()
    : m_impl(NULL)
{
    if (m_impl == NULL)
    {
        m_impl = boost::make_shared<RCFServerImpl>();
    }
}

RCFServerWrapper::~RCFServerWrapper()
{
    // Do nothing
}

void RCFServerWrapper::init(unsigned int port)
{
    if (m_impl != NULL)
    {
        m_impl->init(port);
    }
}

bool RCFServerWrapper::start()
{
    if (m_impl != NULL)
    {
        return m_impl->start();
    }

    return false;
}

bool RCFServerWrapper::stop()
{
    if (m_impl != NULL)
    {
        return m_impl->stop();
    }

    return false;
}

void RCFServerWrapper::deinit()
{
    if (m_impl != NULL)
    {
        m_impl->deinit();
    }
}

void RCFServerWrapper::setMessageHandler(RCFMessageHandler* rcfMessageHandler)
{
    if (m_impl != NULL)
    {
        m_impl->setMessageHandler(rcfMessageHandler);
    }
}
