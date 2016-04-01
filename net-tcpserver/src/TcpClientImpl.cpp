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
#include "CThreadManage.h"
#include "CRealJob.h"

static const unsigned int DefaultNumOfThread = 10;

TcpClientImpl::TcpClientImpl(const std::string &ip, unsigned short port)
    : m_endpoint(boost::asio::ip::address::from_string(ip), port),
      m_tcpSession(m_ioService),
      m_onRecivedMessage(NULL),
      m_onHandleError(NULL)
{
    m_ioServiceThread.reset();
    m_threadManage.reset();
}

TcpClientImpl::~TcpClientImpl()
{
    stop();
}

bool TcpClientImpl::start()
{
    connect();

    if (m_ioServiceThread.use_count() == 0)
    {
        try
        {
            m_ioServiceThread = std::make_shared<std::thread>
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
    try
    {
        m_tcpSession.socket().close();
        m_ioService.stop();
    }
    catch (std::exception& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }

    if (m_ioServiceThread.use_count() != 0)
    {
        if (m_ioServiceThread->joinable())
        {
            m_ioServiceThread->join();
        }
    }

    m_threadManage->terminateAll();

    return true;
}

void TcpClientImpl::setThreadPoolNum(unsigned int num)
{
    if (m_threadManage.use_count() == 0)
    {
        m_threadManage = std::make_shared<CThreadManage>();
        m_threadManage->initThreadNum(DefaultNumOfThread);
    }
}

void TcpClientImpl::setClientParam(const ClientParam &param)
{
    assert(param.m_onRecivedMessage != NULL);
    assert(param.m_onHandleError != NULL);

    m_onHandleError = param.m_onHandleError;
    m_onRecivedMessage = param.m_onRecivedMessage;
    TcpSessionParam tcpSessionParam;
    tcpSessionParam.m_onRecivedMessage = std::bind(&TcpClientImpl::handleReciveMessage,
                                                   this, std::placeholders::_1,
                                                   std::placeholders::_2);
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
        if (m_onHandleError != NULL)
        {
            m_onHandleError(error.message(), "");
        }
        return;
    }

    m_tcpSession.asyncRead();
}

void TcpClientImpl::handleReciveMessage(MessagePtr message, const std::string& remoteAddress)
{
    if (message.use_count() == 0)
    {
        std::cout << "Tcp client message.use_count() == 0" << std::endl;
        return;
    }

    CRealJobPtr job(new CRealJob(m_onRecivedMessage, message, remoteAddress));
    m_threadManage->run(job);
}
