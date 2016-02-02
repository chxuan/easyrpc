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
#include <iostream>

TcpServerImpl::TcpServerImpl(const std::string& ip, unsigned int port)
    : m_acceptor(m_ioService,
        boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
{
    accept();
}

TcpServerImpl::~TcpServerImpl()
{

}

bool TcpServerImpl::start()
{
    try
    {
        m_ioService.run();
    }
    catch (std::exception& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
        return false;
    }

    return true;
}

bool TcpServerImpl::stop()
{
    return true;
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
    if (!error)
    {
        //tcpSession->asyncRead();
    }
    else
    {
        std::cout << "Tcp server accept failed: " << error.message() << std::endl;
    }

    accept();
}
