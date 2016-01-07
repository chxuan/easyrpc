/* Copyright(C)
* For free
* All right reserved
* 
*/
/**
* @file RCFServerImpl.h
* @brief RCF服务端通信框架实现类
* @author highway-9, 787280310@qq.com
* @version 1.1.0
* @date 2016-01-04
*/

#ifndef _RCFSERVERIMPL_H
#define _RCFSERVERIMPL_H

#include <RCF/RCF.hpp>
#include "Message.h"

class RCFMessageImpl
{
public:
    void sendMessage(Message* message, Message* retMessage);
};

class RCFServerImpl
{
public:
    RCFServerImpl();
    ~RCFServerImpl();

    void init(unsigned int port = 50001);
    bool start();
    bool stop();
    void deinit();

private:
    typedef boost::shared_ptr<RCF::RcfInitDeinit> RcfInitDeinitPtr;
    RcfInitDeinitPtr        m_rcfInit;

    typedef boost::shared_ptr<RCF::RcfServer> RcfServerPtr;
    RcfServerPtr            m_rcfServer;
    RCFMessageImpl          m_rcfMessageImpl;
    unsigned int            m_port;
};

#endif
