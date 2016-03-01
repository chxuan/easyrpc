/* Copyright(C)
* For free
* All right reserved
*
*/
/**
* @file TcpServerImpl.h
* @brief tcp服务器包装类
* @author highway-9, 787280310@qq.com
* @version 1.1.0
* @date 2016-01-31
*/

#ifndef _TCPSERVERWRAPPER_H
#define _TCPSERVERWRAPPER_H

#include "TcpServerImpl.h"

class TcpServerWrapper
{
public:
    TcpServerWrapper(unsigned short port);

    bool start();

    bool stop();

    void setThreadPoolNum(unsigned int num);

    void setServerParam(const ServerParam& param);

    template<typename T>
    void write(const T t, const std::string& remoteAddress)
    {
        assert(m_impl.use_count() != 0);
        m_impl->write(t, remoteAddress);
    }

private:
    TcpServerImplPtr m_impl;
};

typedef boost::shared_ptr<TcpServerWrapper> TcpServerWrapperPtr;

#endif
