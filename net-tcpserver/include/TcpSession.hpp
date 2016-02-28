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
#include <iomanip>
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
#include "Message.h"
#include "PeopleInfoMessage.h"

typedef boost::function1<void, MessagePtr> OnReciveMessage;
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
    TcpSession(boost::asio::io_service& ioService)
        : m_socket(ioService),
          m_onReciveMessage(NULL),
          m_onHandleError(NULL)
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

    void setTcpSessionParam(const TcpSessionParam& param)
    {
        assert(param.m_onRecivedMessage != NULL);
        assert(param.m_onHandleError != NULL);
        m_onReciveMessage = param.m_onRecivedMessage;
        m_onHandleError = param.m_onHandleError;
    }

    void asyncRead()
    {
        boost::asio::async_read(m_socket, boost::asio::buffer(m_inboundHeader),
                                boost::bind(&TcpSession::handleReadHeader, this,
                                            boost::asio::placeholders::error));
    }

    template<typename T>
    void write(const T t)
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
    void handleReadHeader(const boost::system::error_code& error)
    {
        if (error)
        {
            std::cout << "Read header failed: " << error.message() << std::endl;
            checkError(error);
            return;
        }

        // 解析header
        std::istringstream is(std::string(m_inboundHeader, HeaderLength));
        std::size_t inboundDataSize = 0;
        if (!(is >> std::hex >> inboundDataSize))
        {
            std::cout << "Header doesn't seem to be valid" << std::endl;
            boost::system::error_code error(boost::asio::error::invalid_argument);
            checkError(error);
            return;
        }

        m_inboundData.clear();
        m_inboundData.resize(inboundDataSize);
        boost::asio::async_read(m_socket, boost::asio::buffer(m_inboundMessageType),
                                boost::bind(&TcpSession::handleReadMessageType, this,
                                            boost::asio::placeholders::error));
    }

    void handleReadMessageType(const boost::system::error_code& error)
    {
        if (error)
        {
            std::cout << "Read message type failed: " << error.message() << std::endl;
            checkError(error);
            return;
        }

        MessagePtr message(new Message);

        // 解析message type
        std::istringstream is(std::string(m_inboundMessageType, MessageTypeLength));
        message->m_messageType = 0;
        if (!(is >> std::hex >> message->m_messageType))
        {
            std::cout << "Mesage type doesn't seem to be valid" << std::endl;
            boost::system::error_code error(boost::asio::error::invalid_argument);
            checkError(error);
            return;
        }

        boost::asio::async_read(m_socket, boost::asio::buffer(m_inboundData),
                                boost::bind(&TcpSession::handleReadData, this,
                                            message, boost::asio::placeholders::error));
    }

    void handleReadData(MessagePtr message, const boost::system::error_code& error)
    {
        asyncRead();

        if (error)
        {
            std::cout << "Read message data failed: " << error.message() << std::endl;
            checkError(error);
            return;
        }

        message->m_data = std::string(&m_inboundData[0], m_inboundData.size());
        if (m_onReciveMessage != NULL)
        {
            m_onReciveMessage(message);
        }
    }

    void checkError(const boost::system::error_code& error)
    {
        if (error)
        {
            if (m_onHandleError != NULL)
            {
                std::string errorString = error.message();
                if (errorString == "End of file")
                {
                    m_onHandleError(error, remoteAddress());
                }
                else
                {
                    m_onHandleError(error, "");
                }
            }
        }
    }

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
};

typedef boost::shared_ptr<TcpSession> TcpSessionPtr;

#endif
