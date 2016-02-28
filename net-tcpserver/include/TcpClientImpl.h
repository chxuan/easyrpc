/* Copyright(C)
* For free
* All right reserved
*
*/
/**
* @file TcpClientImpl.h
* @brief tcp客户端实现类
* @author highway-9, 787280310@qq.com
* @version 1.1.0
* @date 2016-01-31
*/

#ifndef _TCPCLIENT_H
#define _TCPCLIENT_H

#include "TcpSession.hpp"

class CThreadManage;
typedef boost::shared_ptr<CThreadManage> CThreadManagePtr;

class ClientParam
{
public:
    ClientParam()
        : m_onRecivedMessage(NULL),
          m_onHandleError(NULL)
    {
        // Do nothing
    }

    OnReciveMessage m_onRecivedMessage;
    OnHandleError m_onHandleError;
};

class TcpClientImpl
{
public:
    TcpClientImpl(const std::string& ip, unsigned short port);
    ~TcpClientImpl();

    bool start();
    bool stop();

    template<typename T>
    void write(const T t)
    {
        m_tcpSession.write(t);
    }

    void setThreadPoolNum(unsigned int num);

    void setClientParam(const ClientParam& param);

private:
    void connect();

    void handleConnect(const boost::system::error_code& error);

    void handleReciveMessage(MessagePtr message, const std::string& remoteAddress);

private:
    boost::asio::io_service m_ioService;
    boost::asio::ip::tcp::endpoint m_endpoint;

    typedef boost::shared_ptr<boost::thread> ThreadPtr;
    ThreadPtr m_ioServiceThread;

    TcpSession m_tcpSession;

    CThreadManagePtr m_threadManage;

    OnReciveMessage m_onRecivedMessage;
    OnHandleError m_onHandleError;
};

typedef boost::shared_ptr<TcpClientImpl> TcpClientImplPtr;

#endif
