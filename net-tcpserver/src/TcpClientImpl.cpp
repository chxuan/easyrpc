/* Copyright(C)
* For free
* All right reserved
*
*/
/**
* @file TcpClientImpl.cpp
* @brief tcp客户端实现
* @author highway-9, 787280310@qq.com
* @version 1.1.0
* @date 2016-01-31
*/

#include "TcpClientImpl.h"

TcpClientImpl::TcpClientImpl(const std::string &ip, unsigned short port)
    : m_endpoint(boost::asio::ip::address::from_string(ip), port),
      m_tcpSession(m_ioService),
      m_onHandleError(NULL)
{
    m_ioServiceThread.reset();
    connect();
}

TcpClientImpl::~TcpClientImpl()
{
    stop();
}

bool TcpClientImpl::start()
{
    if (m_ioServiceThread.use_count() == 0)
    {
        try
        {
            m_ioServiceThread = boost::make_shared<boost::thread>
                    (boost::bind(&boost::asio::io_service::run, &m_ioService));
        }
        catch (std::exception& e)
        {
            std::cout << "Error: " << e.what() << std::endl;
            return false;
        }
    }

    return true;
}

bool TcpClientImpl::stop()
{
    m_ioService.stop();
    joinIOServiceThread();

    return true;
}

void TcpClientImpl::setClientParam(const ClientParam &param)
{
    assert(param.m_onRecivedMessage != NULL);
    assert(param.m_onHandleError != NULL);

    m_onHandleError = param.m_onHandleError;
    TcpSessionParam tcpSessionParam;
    tcpSessionParam.m_onRecivedMessage = param.m_onRecivedMessage;
    tcpSessionParam.m_onHandleError = param.m_onHandleError;
    m_tcpSession.setTcpSessionParam(tcpSessionParam);
}

void TcpClientImpl::connect()
{
    m_tcpSession.socket().async_connect(m_endpoint,
                                        boost::bind(&TcpClientImpl::handleConnect, this,
                                                    boost::asio::placeholders::error));
}

void TcpClientImpl::handleConnect(const boost::system::error_code &error)
{
    if (error)
    {
        std::cout << error.message() << std::endl;
        std::cout << __FUNCTION__ << " " << __LINE__ << std::endl;
        if (m_onHandleError != NULL)
        {
            m_onHandleError(error);
        }
        return;
    }

    m_tcpSession.asyncRead();
}

void TcpClientImpl::joinIOServiceThread()
{
    if (m_ioServiceThread.use_count() != 0)
    {
        if (m_ioServiceThread->joinable())
        {
            m_ioServiceThread->join();
        }
    }
}
