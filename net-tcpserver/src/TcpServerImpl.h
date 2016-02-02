/* Copyright(C)
* For free
* All right reserved
* 
*/
/**
* @file TcpServerImpl.h
* @brief tcp服务器实现类
* @author highway-9, 787280310@qq.com
* @version 1.1.0
* @date 2016-01-31
*/

#ifndef _TCPSERVER_H
#define _TCPSERVER_H

#include "TcpSession.hpp"

class TcpServerImpl
{
public:
    TcpServerImpl(const std::string& ip, unsigned int port);
    ~TcpServerImpl();

    bool start();
    bool stop();

private:
    void accept();

    void handleAccept(TcpSessionPtr tcpSession,
                      const boost::system::error_code& error);

private:
    boost::asio::io_service m_ioService;
    boost::asio::ip::tcp::acceptor m_acceptor;
};

#endif
