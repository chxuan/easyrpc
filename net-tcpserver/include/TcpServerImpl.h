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
    TcpServerImpl(unsigned short port);
    ~TcpServerImpl();

    bool start();
    bool stop();

    template<typename T>
    void asyncWrite(const T t, const std::string& remoteAddress)
    {
        TcpSessionPtr session = tcpSession(remoteAddress);
        if (session != NULL)
        {
            session->asyncWrite(t);
        }
    }

    std::vector<std::string> allRemoteAddress();

private:
    void accept();

    void handleAccept(TcpSessionPtr tcpSession,
                      const boost::system::error_code& error);

    void closeAllTcpSession();

    void joinIOServiceThread();

    bool isTcpSessionExists(const std::string& remoteAddress);

    TcpSessionPtr tcpSession(const std::string& remoteAddress);

private:
    boost::asio::io_service m_ioService;
    boost::asio::ip::tcp::acceptor m_acceptor;

    typedef boost::shared_ptr<boost::thread> ThreadPtr;
    ThreadPtr m_ioServiceThread;

    // key: ip:port(127.0.0.1:8888)
    typedef std::unordered_map<std::string, TcpSessionPtr> TcpSessionMap;
    TcpSessionMap m_tcpSessionMap;
};

#endif
