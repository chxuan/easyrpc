/* Copyright(C)
* For free
* All right reserved
* 
*/
/**
* @file TcpServerImpl.cpp
* @brief tcp服务器实现
* @author highway-9, 787280310@qq.com
* @version 1.1.0
* @date 2016-01-31
*/

#include "TcpServerImpl.h"
#include "CThreadManage.h"
#include "CRealJob.h"
#include <iostream>

static const unsigned int DefaultNumOfThread = 10;

TcpServerImpl::TcpServerImpl(unsigned short port)
    : m_acceptor(m_ioService,
      boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
      m_onRecivedMessage(NULL),
      m_onHandleError(NULL),
      m_onClientConnect(NULL),
      m_onClientDisconnect(NULL)
{
    m_ioServiceThread.reset();
    m_threadManage.reset();
}

TcpServerImpl::~TcpServerImpl()
{
    stop();
}

bool TcpServerImpl::start()
{
    accept();

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

bool TcpServerImpl::stop()
{
    closeAllTcpSession();

    try
    {
        m_ioService.stop();
    }
    catch (std::exception& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
        return false;
    }

    if (m_ioServiceThread.use_count() != 0)
    {
        if (m_ioServiceThread->joinable())
        {
            m_ioServiceThread->join();
        }
    }

    return true;
}

void TcpServerImpl::setThreadPoolNum(unsigned int num)
{
    if (m_threadManage.use_count() == 0)
    {
        m_threadManage = boost::make_shared<CThreadManage>();
        m_threadManage->initThreadNum(num);
    }
}

void TcpServerImpl::setServerParam(const ServerParam &param)
{
    assert(param.m_onRecivedMessage != NULL);
    assert(param.m_onHandleError != NULL);
    assert(param.m_onClientConnect != NULL);
    assert(param.m_onClientDisconnect != NULL);

    m_onRecivedMessage = param.m_onRecivedMessage;
    m_onHandleError = param.m_onHandleError;
    m_onClientConnect = param.m_onClientConnect;
    m_onClientDisconnect = param.m_onClientDisconnect;
}

void TcpServerImpl::accept()
{
    TcpSessionPtr tcpSession(new TcpSession(m_ioService));
    m_acceptor.async_accept(tcpSession->socket(),
        boost::bind(&TcpServerImpl::handleAccept, this, tcpSession,
                    boost::asio::placeholders::error));
}

void TcpServerImpl::handleAccept(TcpSessionPtr tcpSession,
                                 const boost::system::error_code &error)
{
    accept();

    if (!error)
    {
        TcpSessionParam tcpSessionParam;
        tcpSessionParam.m_onRecivedMessage = boost::bind(&TcpServerImpl::handleReciveMessage, this, _1, _2);
        tcpSessionParam.m_onHandleError = boost::bind(&TcpServerImpl::handleError, this, _1, _2);
        tcpSession->setTcpSessionParam(tcpSessionParam);

        std::string remoteAddress = tcpSession->remoteAddress();
        {
            boost::lock_guard<boost::mutex> locker(m_sessionMapMutex);
            m_tcpSessionMap.insert(std::make_pair(remoteAddress, tcpSession));
        }
        if (m_onClientConnect != NULL)
        {
            m_onClientConnect(remoteAddress);
        }

        tcpSession->asyncRead();
    }
    else
    {
        std::cout << "Tcp server accept failed: " << error.message() << std::endl;
        if (m_onHandleError != NULL)
        {
            m_onHandleError(error, "");
        }
    }
}

void TcpServerImpl::closeAllTcpSession()
{
    boost::lock_guard<boost::mutex> locker(m_sessionMapMutex);
    auto begin = m_tcpSessionMap.begin();
    auto end = m_tcpSessionMap.end();
    while (begin != end)
    {
        TcpSessionPtr session = begin->second;
        try
        {
            session->socket().close();
        }
        catch (std::exception& e)
        {
            std::cout << "Error: " << e.what() << std::endl;
        }

        ++begin;
    }

    m_tcpSessionMap.clear();
}

TcpSessionPtr TcpServerImpl::tcpSession(const std::string& remoteAddress)
{
    boost::lock_guard<boost::mutex> locker(m_sessionMapMutex);
    auto iter = m_tcpSessionMap.find(remoteAddress);
    if (iter != m_tcpSessionMap.end())
    {
        return iter->second;
    }

    return TcpSessionPtr();
}

void TcpServerImpl::handleReciveMessage(MessagePtr message, const std::string& remoteAddress)
{
    if (message.use_count() == 0)
    {
        std::cout << "Tcp server message.use_count() == 0" << std::endl;
        return;
    }

    CRealJobPtr job(new CRealJob(m_onRecivedMessage, message, remoteAddress));
    m_threadManage->run(job);
}

void TcpServerImpl::handleError(const boost::system::error_code &error, const std::string &remoteAddress)
{
    if (error)
    {
        std::string errorString = error.message();
        if (errorString == "End of file")
        {
            closeTcpSession(remoteAddress);
            if (m_onClientDisconnect != NULL)
            {
                m_onClientDisconnect(remoteAddress);
            }
        }
        else
        {
            if (m_onHandleError != NULL)
            {
                m_onHandleError(error, remoteAddress);
            }
        }
    }
}

bool TcpServerImpl::closeTcpSession(const std::string &remoteAddress)
{
    boost::lock_guard<boost::mutex> locker(m_sessionMapMutex);
    auto iter = m_tcpSessionMap.find(remoteAddress);
    if (iter != m_tcpSessionMap.end())
    {
        TcpSessionPtr session = iter->second;
        try
        {
            session->socket().close();
            m_tcpSessionMap.erase(iter);
            return true;
        }
        catch (std::exception& e)
        {
            std::cout << "Error: " << e.what() << std::endl;
            m_tcpSessionMap.erase(iter);
            return false;
        }
    }

    return false;
}
