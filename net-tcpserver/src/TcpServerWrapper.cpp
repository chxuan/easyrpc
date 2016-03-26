/* Copyright(C)
* For free
* All right reserved
*
*/
/**
* @file TcpServerImpl.cpp
* @brief tcp服务器包装类
* @author highway-9, 787280310@qq.com
* @version 1.1.0
* @date 2016-01-31
*/

#include "TcpServerWrapper.h"

TcpServerWrapper::TcpServerWrapper(const std::string& ip, unsigned short port)
{
    m_impl.reset();
    if (m_impl.use_count() == 0)
    {
        m_impl = std::make_shared<TcpServerImpl>(ip, port);
    }
}

bool TcpServerWrapper::start()
{
    assert(m_impl.use_count() != 0);
    return m_impl->start();
}

bool TcpServerWrapper::stop()
{
    assert(m_impl.use_count() != 0);
    return m_impl->stop();
}

void TcpServerWrapper::setThreadPoolNum(unsigned int num)
{
    assert(m_impl.use_count() != 0);
    m_impl->setThreadPoolNum(num);
}

void TcpServerWrapper::setServerParam(const ServerParam &param)
{
    assert(m_impl.use_count() != 0);
    m_impl->setServerParam(param);
}
