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

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
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
#include "Message.h"

class TcpSession
{
public:
    TcpSession(boost::asio::io_service& ioService) : m_socket(ioService)
    {
        // Do nothing
    }

    boost::asio::ip::tcp::socket &socket() { return m_socket; }
    std::string remoteIp() { return m_socket.remote_endpoint().address().to_string(); }
    unsigned short remotePort() { return m_socket.remote_endpoint().port(); }

    std::string remoteAddress()
    {
        std::ostringstream os;
        os << remoteIp() << ":" << remotePort();
        return os.str();
    }

    void asyncRead(Message* message)
    {
        boost::asio::async_read(m_socket, boost::asio::buffer(m_inboundHeader),
                                boost::bind(&TcpSession::handleReadHeader, this,
                                            message, boost::asio::placeholders::error));
    }

    template<typename T>
    void asyncWrite(const T* t)
    {
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
        messageTypeStream << std::setw(MesageTypeLength)
          << std::hex << t->m_messageType;
        if (!messageTypeStream || messageTypeStream.str().size() != MesageTypeLength)
        {
            std::cout << "Format the message type failed" << std::endl;
            return;
        }
        m_outboundMessageType = messageTypeStream.str();

        std::vector<boost::asio::const_buffer> buffers;
        buffers.push_back(boost::asio::buffer(m_outboundHeader));
        buffers.push_back(boost::asio::buffer(m_outboundMessageType));
        buffers.push_back(boost::asio::buffer(m_outboundData));
        boost::asio::async_write(m_socket, buffers, boost::bind(&TcpSession::handleWrite, this,
                                                                boost::asio::placeholders::error));
    }

private:
    void handleReadHeader(Message* message, const boost::system::error_code& error)
    {
        if (error)
        {
            std::cout << "Read header failed: " << error.message() << std::endl;
            return;
        }

        // 解析header
        std::istringstream is(std::string(m_inboundHeader, HeaderLength));
        std::size_t inboundDataSize = 0;
        if (!(is >> std::hex >> inboundDataSize))
        {
            std::cout << "Header doesn't seem to be valid" << std::endl;
            return;
        }

        m_inboundData.clear();
        m_inboundData.resize(inboundDataSize);
        boost::asio::async_read(m_socket, boost::asio::buffer(m_inboundData),
                                boost::bind(&TcpSession::handleReadMessageType, this,
                                            message, boost::asio::placeholders::error));
    }

    void handleReadMessageType(Message* message, const boost::system::error_code& error)
    {
        if (error)
        {
            std::cout << "Read message type failed: " << error.message() << std::endl;
            return;
        }

        // 解析message type
        std::istringstream is(std::string(m_inboundMessageType, MesageTypeLength));
        message->m_messageType = 0;
        if (!(is >> std::hex >> message->m_messageType))
        {
            std::cout << "Mesage type doesn't seem to be valid" << std::endl;
            return;
        }

        boost::asio::async_read(m_socket, boost::asio::buffer(m_inboundData),
                                boost::bind(&TcpSession::handleReadData, this,
                                            message, boost::asio::placeholders::error));
    }

    void handleReadData(Message* message, const boost::system::error_code& error)
    {
        if (error)
        {
            std::cout << "Read data failed: " << error.message() << std::endl;
            return;
        }

        message->m_data = std::string(&m_inboundData[0], m_inboundData.size());
    }

    void handleWrite(const boost::system::error_code& error)
    {
        if (error)
        {
            std::cout << "Write message failed: " << error.message() << std::endl;
        }

        // Nothing to do. The socket will be closed automatically when the last
        // reference to the connection object goes away.
    }

private:
    boost::asio::ip::tcp::socket m_socket;

    enum
    {
        HeaderLength = 8,
        MesageTypeLength = 8
    };

    std::string m_outboundHeader;
    std::string m_outboundMessageType;
    std::string m_outboundData;

    char m_inboundHeader[HeaderLength];
    char m_inboundMessageType[MesageTypeLength];
    std::vector<char> m_inboundData;
};

typedef boost::shared_ptr<TcpSession> TcpSessionPtr;

#endif
