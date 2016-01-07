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

class RCFServerImpl;

class RCFMessageImpl
{
public:
    RCFMessageImpl(const RCFServerImpl* impl);
    ~RCFMessageImpl();

    void sendMessage(Message* message, Message* retMessage);

private:
    RCFServerImpl*      m_impl;
};

class RCFServerImpl
{
public:
    RCFServerImpl();
    ~RCFServerImpl();

    /**
    * @brief init 初始化RCF服务器端
    *
    * @param port 监听端口，默认为50001
    */
    void init(unsigned int port = 50001);

    /**
    * @brief start 开始服务器
    *
    * @return 成功返回true，否则返回false
    */
    bool start();

    /**
    * @brief stop 停止服务器
    *
    * @return 成功返回true，否则返回false
    */
    bool stop();

    /**
    * @brief deinit 反初始化，释放一些资源
    */
    void deinit();

    /**
    * @brief setMessageCallback 设置消息回调函数
    *
    * @param func 回调函数指针
    */
    void setMessageCallback(MESSAGE_CALLBACK func);

public:
    MESSAGE_CALLBACK         m_messageCallback;         ///< 消息回调函数

private:
    typedef boost::shared_ptr<RCF::RcfInitDeinit> RcfInitDeinitPtr;
    RcfInitDeinitPtr        m_rcfInit;

    typedef boost::shared_ptr<RCF::RcfServer> RcfServerPtr;
    RcfServerPtr            m_rcfServer;
    RCFMessageImpl          m_rcfMessageImpl;
    unsigned int            m_port;                    ///< 监听端口
};

#endif
