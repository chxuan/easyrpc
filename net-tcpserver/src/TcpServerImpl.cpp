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

typedef boost::shared_ptr<TcpSession> chat_session_ptr;

TcpServerImpl::TcpServerImpl(const string &ip, unsigned int port)
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

}

void TcpServerImpl::accept()
{

}
