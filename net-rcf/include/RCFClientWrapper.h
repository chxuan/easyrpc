/* Copyright(C)
* For free
* All right reserved
* 
*/
/**
* @file RCFClientWrapper.h
* @brief RCF客户端通信框架包装类
* @author highway-9, 787280310@qq.com
* @version 1.1.0
* @date 2016-01-04
*/

#ifndef _RCFCLIENTWRAPPER_H
#define _RCFCLIENTWRAPPER_H

#include <boost/shared_ptr.hpp>

class Message;
class RCFClientImpl;

/**
* @brief RCF客户的通信框架包装类
*/
class RCFClientWrapper
{
public:
    /**
    * @brief RCFClientWrapper 构造函数
    */
    RCFClientWrapper();

    /**
    * @brief ~RCFClientWrapper 析构函数
    */
    ~RCFClientWrapper();

    /**
    * @brief init 初始化RCF客户端
    *
    * @param ip 服务器ip地址
    * @param port 服务器端口号，默认为50001
    */
    void init(const std::string& ip, unsigned int port = 50001);

    /**
    * @brief sendMessage 发送消息到服务器端
    *
    * @param message 发送的消息
    * @param retMessage 接收的消息
    *
    * @return 成功返回true，否则返回false
    */
    bool sendMessage(Message* message, Message* retMessage);

    /**
    * @brief deinit 反初始化，释放一些资源
    */
    void deinit();

private:
    typedef boost::shared_ptr<RCFClientImpl> RCFClientImplPtr;
    RCFClientImplPtr        m_impl;		///< RCF客户端实现类指针
};

#endif
