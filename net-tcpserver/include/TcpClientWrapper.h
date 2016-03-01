/* Copyright(C)
* For free
* All right reserved
*
*/
/**
* @file TcpClientImpl.h
* @brief tcp客户端包装类
* @author highway-9, 787280310@qq.com
* @version 1.1.0
* @date 2016-01-31
*/

#ifndef _TCPCLIENTWRAPPER_H
#define _TCPCLIENTWRAPPER_H

#include "TcpClientImpl.h"

class TcpClientWrapper
{
public:
    TcpClientWrapper(const std::string& ip, unsigned short port);

    bool start();
    bool stop();

    void setThreadPoolNum(unsigned int num);

    void setClientParam(const ClientParam& param);

    template<typename T>
    void write(const T t)
    {
        assert(m_impl.use_count() != 0);
        m_impl->write(t);
    }

private:
    TcpClientImplPtr m_impl;
};

typedef boost::shared_ptr<TcpClientWrapper> TcpClientWrapperPtr;

#endif
