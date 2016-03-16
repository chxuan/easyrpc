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

/**
* @brief 会话参数，设置接收消息、错误处理回调函数
*/
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

/**
* @brief 消息头部，包含消息体大小、消息类型
*/
class Header
{
public:
    Header()
        : m_dataSize(0),
          m_messageType(0)
    {
        // Do nothing
    }

    unsigned int m_dataSize;
    unsigned int m_messageType;
};

/**
* @brief tcp会话类，提供服务器与客户端的通信接口
*/
class TcpSession
{
public:
    TcpSession(boost::asio::io_service& ioService);

    /**
    * @brief setTcpSessionParam 设置会话参数
    *
    * @param param 会话参数
    */
    void setTcpSessionParam(const TcpSessionParam& param);

    /**
    * @brief socket 获取socket
    *
    * @return socket
    */
    boost::asio::ip::tcp::socket& socket();

    /**
    * @brief remoteIp 获取ip地址
    *
    * @return ip地址
    */
    std::string remoteIp();

    /**
    * @brief remotePort 获取端口号
    *
    * @return 端口号
    */
    unsigned short remotePort();

    /**
    * @brief remoteAddress 获取远端地址（地址格式：127.0.0.1:8888）
    *
    * @return 远端地址
    */
    std::string remoteAddress();

    /**
    * @brief asyncRead 异步读取消息
    */
    void asyncRead();

    /**
    * @brief write 同步写消息
    *
    * @tparam T 摸板类型
    * @param t 消息结构
    */
    template<typename T>
    void write(const T t)
    {
        boost::lock_guard<boost::mutex> locker(m_writeMutex);

        // 序列化数据
        std::ostringstream archiveStream;
        try
        {
            boost::archive::binary_oarchive archive(archiveStream);
            archive << *t;
        }
        catch (std::exception& e)
        {
            std::cout << "Serialize data failed: " << e.what() << std::endl;
            return;
        }

        std::string messageData = archiveStream.str();

        Header header;
        header.m_dataSize = messageData.size();
        header.m_messageType = t->m_messageType;

        char headerBuf[HeaderLength] = {'\0'};
        memcpy(headerBuf, &header, sizeof(headerBuf));

        std::vector<boost::asio::const_buffer> buffers;
        buffers.push_back(boost::asio::buffer(headerBuf));
        buffers.push_back(boost::asio::buffer(messageData));

        boost::system::error_code error;
        boost::asio::write(m_socket, buffers, error);
        checkError(error);
    }

private:
    /**
    * @brief handleReadHeader 处理读取的消息头
    *
    * @param error 错误类型
    */
    void handleReadHeader(const boost::system::error_code& error);

    /**
    * @brief handleReadData 处理读取的消息体
    *
    * @param message 消息
    * @param error 错误类型
    */
    void handleReadData(MessagePtr message, const boost::system::error_code& error);

    /**
    * @brief checkError 判断是什么类型的错误，然后做相应的处理
    *
    * @param error 错误类型
    */
    void checkError(const boost::system::error_code& error);

private:
    boost::asio::ip::tcp::socket m_socket;

    enum
    {
        HeaderLength = 8
    };

    char m_inboundHeader[HeaderLength];
    std::vector<char> m_inboundData;

    OnReciveMessage m_onReciveMessage;
    OnHandleError m_onHandleError;

    boost::mutex m_writeMutex;
};

typedef boost::shared_ptr<TcpSession> TcpSessionPtr;

#endif
