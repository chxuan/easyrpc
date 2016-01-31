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

#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <boost/asio.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/thread.hpp>

#include "TcpSession.h"

class TcpServerImpl
{
public:
    TcpServerImpl(const std::string& ip, unsigned int port);
    ~TcpServerImpl();

    bool start();
    bool stop();

private:
    void accept();

private:
    boost::asio::io_service m_ioService;
    boost::asio::ip::tcp::acceptor m_acceptor;
};

#endif
