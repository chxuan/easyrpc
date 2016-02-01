/* Copyright(C)
* For free
* All right reserved
* 
*/
/**
* @file TcpSession.cpp
* @brief tcp会话实现
* @author highway-9, 787280310@qq.com
* @version 1.1.0
* @date 2016-01-31
*/

#include "TcpSession.h"
#include <iostream>
#include <iomanip>
#include <sstream>
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

TcpSession::TcpSession(boost::asio::io_service &ioService)
    : m_socket(ioService)
{
    // Do nothing
}

TcpSession::~TcpSession()
{

}

boost::asio::ip::tcp::socket &TcpSession::socket()
{
    return m_socket;
}

void TcpSession::asyncRead(TcpSession::T &t)
{
    boost::asio::async_read(m_socket, boost::asio::buffer(m_inboundHeader),
                            boost::bind(&TcpSession::handleReadHeader, this,
                                        t, boost::asio::placeholders::error));
}

void TcpSession::asyncWrite(const TcpSession::T &t)
{
    // 序列化数据
    std::ostringstream archiveStream;
    boost::archive::binary_oarchive archive(archiveStream);
    archive << t;
    m_outboundData = archiveStream.str();

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

    std::vector<boost::asio::const_buffer> buffers;
    buffers.push_back(boost::asio::buffer(m_outboundHeader));
    buffers.push_back(boost::asio::buffer(m_outboundData));
    boost::asio::async_write(m_socket, buffers, boost::bind(&TcpSession::handleWrite, this,
                                                            boost::asio::placeholders::error));
}

void TcpSession::handleReadHeader(TcpSession::T &t,
                                  const boost::system::error_code &error)
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

    boost::asio::async_read(m_socket, boost::asio::buffer(m_inboundData),
                            boost::bind(&TcpSession::handleReadData, this,
                                        t, boost::asio::placeholders::error));
}

void TcpSession::handleReadData(TcpSession::T &t,
                                const boost::system::error_code &error)
{
    if (error)
    {
        std::cout << "Read data failed: " << error.message() << std::endl;
        return;
    }

    // 反序列化数据
    try
    {
        std::string archiveData(&m_inboundData[0], m_inboundData.size());
        std::istringstream archiveStream(archiveData);
        boost::archive::binary_oarchive archive(archiveStream);
        archive >> t;
    }
    catch (std::exception& e)
    {
        (void)e;
        std::cout << "Unable to decode data" << std::endl;
        return;
    }
}

void TcpSession::handleWrite(const boost::system::error_code &error)
{
    if (error)
    {
        std::cout << "Write message failed: " << error.message() << std::endl;
    }

    // Nothing to do. The socket will be closed automatically when the last
    // reference to the connection object goes away.
}
