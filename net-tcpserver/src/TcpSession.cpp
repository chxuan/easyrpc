/* Copyright(C)
* For free
* All right reserved
*
*/
/**
* @file TcpSession.cpp
* @brief tcp会话
* @author highway-9, 787280310@qq.com
* @version 1.1.0
* @date 2016-01-31
*/

#include "TcpSession.h"
#include "Message.h"

TcpSession::TcpSession(boost::asio::io_service& ioService)
    : m_socket(ioService),
      m_onReciveMessage(NULL),
      m_onHandleError(NULL)
{
    // Do nothing
}

void TcpSession::setTcpSessionParam(const TcpSessionParam& param)
{
    assert(param.m_onRecivedMessage != NULL);
    assert(param.m_onHandleError != NULL);
    m_onReciveMessage = param.m_onRecivedMessage;
    m_onHandleError = param.m_onHandleError;
}

boost::asio::ip::tcp::socket& TcpSession::socket()
{
    return m_socket;
}

std::string TcpSession::remoteIp()
{
    return m_socket.remote_endpoint().address().to_string();
}

unsigned short TcpSession::remotePort()
{
    return m_socket.remote_endpoint().port();
}

std::string TcpSession::remoteAddress()
{
    std::ostringstream os;
    os << remoteIp() << ":" << remotePort();
    return os.str();
}

void TcpSession::asyncRead()
{
    boost::asio::async_read(m_socket, boost::asio::buffer(m_inboundHeader),
                            boost::bind(&TcpSession::handleReadHeader, this,
                                        boost::asio::placeholders::error));
}

void TcpSession::handleReadHeader(const boost::system::error_code& error)
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

void TcpSession::handleReadMessageType(const boost::system::error_code& error)
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

void TcpSession::handleReadData(MessagePtr message, const boost::system::error_code& error)
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
        m_onReciveMessage(message, remoteAddress());
    }
}

void TcpSession::checkError(const boost::system::error_code& error)
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

