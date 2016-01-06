/* Copyright(C)
* For free
* All right reserved
* 
*/
/**
* @file ThriftServerWrapper.cpp
* @brief thrift服务器端通信包装实现文件
* @author highway-9, 787280310@qq.com
* @version 1.1.0
* @date 2015-11-25
*/

#include "ThriftServerWrapper.h"
#include "ThriftServerImpl.h"

ThriftServerWrapper::ThriftServerWrapper()
    : m_impl(NULL)
{
    if (m_impl == NULL)
    {
        m_impl = std::make_shared<ThriftServerImpl>();
    }
}

ThriftServerWrapper::~ThriftServerWrapper()
{
    // Do nothing
}

void ThriftServerWrapper::init(unsigned int port)
{
    if (m_impl != NULL)
    {
        m_impl->init(port);
    }
}

bool ThriftServerWrapper::start()
{
    if (m_impl != NULL)
    {
        return m_impl->start();
    }

    return false;
}

bool ThriftServerWrapper::stop()
{
    if (m_impl != NULL)
    {
        return m_impl->stop();
    }

    return false;
}

void ThriftServerWrapper::deinit()
{
    if (m_impl != NULL)
    {
        m_impl->deinit();
    }
}

void ThriftServerWrapper::setMessageCallback(MESSAGE_CALLBACK func)
{
    if (m_impl != NULL)
    {
        m_impl->setMessageCallback(func);
    }
}
