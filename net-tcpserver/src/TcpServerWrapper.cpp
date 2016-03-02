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

TcpServerWrapper::TcpServerWrapper(unsigned short port)
    : m_isBindSuccess(true)
{
    m_impl.reset();
    if (m_impl.use_count() == 0)
    {
        try
        {
            m_impl = boost::make_shared<TcpServerImpl>(port);
        }
        catch (std::exception& e)
        {
            m_isBindSuccess = false;
            std::cout << "Error: " << e.what() << std::endl;
            return;
        }
    }
}

bool TcpServerWrapper::start()
{
    assert(m_impl.use_count() != 0 && m_isBindSuccess);
    return m_impl->start();
}

bool TcpServerWrapper::stop()
{
    assert(m_impl.use_count() != 0 && m_isBindSuccess);
    return m_impl->stop();
}

void TcpServerWrapper::setThreadPoolNum(unsigned int num)
{
    assert(m_impl.use_count() != 0 && m_isBindSuccess);
    m_impl->setThreadPoolNum(num);
}

void TcpServerWrapper::setServerParam(const ServerParam &param)
{
    assert(m_impl.use_count() != 0 && m_isBindSuccess);
    m_impl->setServerParam(param);
}
