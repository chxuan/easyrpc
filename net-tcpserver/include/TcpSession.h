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

#include <assert.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/set.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/unordered_set.hpp>

class Message;

typedef boost::shared_ptr<Message> MessagePtr;
typedef boost::function2<void, MessagePtr, const std::string&> OnReciveMessage;
typedef boost::function2<void, const boost::system::error_code&, const std::string&> OnHandleError;

class TcpSessionParam
{
public:
    TcpSessionParam()
        : m_onRecivedMessage(NULL),
          m_onHandleError(NULL)
    {
        // Do nothing
    }

    OnReciveMessage m_onRecivedMessage;
    OnHandleError m_onHandleError;
};

class TcpSession
{
public:
    TcpSession(boost::asio::io_service& ioService);

    void setTcpSessionParam(const TcpSessionParam& param);

    boost::asio::ip::tcp::socket& socket();
    std::string remoteIp();
    unsigned short remotePort();
    std::string remoteAddress();

    void asyncRead();

    template<typename T>
    void write(const T t)
    {
        boost::lock_guard<boost::mutex> locker(m_writeMutex);
        // 序列化数据
        try
        {
            std::ostringstream archiveStream;
            boost::archive::binary_oarchive archive(archiveStream);
            archive << *t;
            m_outboundData = archiveStream.str();
        }
        catch (std::exception& e)
        {
            std::cout << "Serialize data failed: " << e.what() << std::endl;
            return;
        }

        // 格式化header
        std::ostringstream headerStream;
        headerStream << std::setw(HeaderLength)
          << std::hex << m_outboundData.size();
        if (!headerStream || headerStream.str().size() != HeaderLength)
        {
            std::cout << "Format the header failed" << std::endl;
            return;
        }
        m_outboundHeader = headerStream.str();

        // 格式化MessageType
        std::ostringstream messageTypeStream;
        messageTypeStream << std::setw(MessageTypeLength)
          << std::hex << t->m_messageType;
        if (!messageTypeStream || messageTypeStream.str().size() != MessageTypeLength)
        {
            std::cout << "Format the message type failed" << std::endl;
            return;
        }
        m_outboundMessageType = messageTypeStream.str();

        std::vector<boost::asio::const_buffer> buffers;
        buffers.push_back(boost::asio::buffer(m_outboundHeader));
        buffers.push_back(boost::asio::buffer(m_outboundMessageType));
        buffers.push_back(boost::asio::buffer(m_outboundData));

        boost::system::error_code error;
        boost::asio::write(m_socket, buffers, error);
        checkError(error);
    }

private:
    void handleReadHeader(const boost::system::error_code& error);
    void handleReadMessageType(const boost::system::error_code& error);
    void handleReadData(MessagePtr message, const boost::system::error_code& error);
    void checkError(const boost::system::error_code& error);

private:
    boost::asio::ip::tcp::socket m_socket;

    enum
    {
        HeaderLength = 8,
        MessageTypeLength = 8
    };

    std::string m_outboundHeader;
    std::string m_outboundMessageType;
    std::string m_outboundData;

    char m_inboundHeader[HeaderLength];
    char m_inboundMessageType[MessageTypeLength];
    std::vector<char> m_inboundData;

    OnReciveMessage m_onReciveMessage;
    OnHandleError m_onHandleError;

    boost::mutex m_writeMutex;
};

typedef boost::shared_ptr<TcpSession> TcpSessionPtr;

#endif
