/* Copyright(C)
* For free
* All right reserved
* 
*/
/**
* @file TcpSession.h
* @brief tcp会话类
* @author highway-9, 787280310@qq.com
* @version 1.1.0
* @date 2016-01-31
*/

#ifndef _TCPSESSION_H
#define _TCPSESSION_H

#include <string>
#include <vector>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>

class TcpSession
{
public:
    TcpSession(boost::asio::io_service& ioService);
    ~TcpSession();

    boost::asio::ip::tcp::socket &socket();

    template<typename T>
    void asyncRead(T& t);

    template<typename T>
    void asyncWrite(const T& t);

private:
    template<typename T>
    void handleReadHeader(T& t, const boost::system::error_code& error);

    template<typename T>
    void handleReadData(T& t, const boost::system::error_code& error);

    void handleWrite(const boost::system::error_code& error);

private:
    boost::asio::ip::tcp::socket m_socket;

    enum { HeaderLength = 8 };

    std::string m_outboundHeader;
    std::string m_outboundData;

    char m_inboundHeader[HeaderLength];
    std::vector<char> m_inboundData;
};

#endif
