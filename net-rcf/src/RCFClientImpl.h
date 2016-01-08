/* Copyright(C)
* For free
* All right reserved
* 
*/
/**
* @file RCFClientImpl.h
* @brief RCF客户端通信框架实现类
* @author highway-9, 787280310@qq.com
* @version 1.1.0
* @date 2016-01-04
*/

#ifndef _RCFCLIENTIMPL_H
#define _RCFCLIENTIMPL_H

#include <RCF/RCF.hpp>

/**
* @brief RCF客户端通信框架实现类 
*/
class RCFClientImpl
{
public:
    /**
    * @brief RCFClientImpl 构造函数
    */
    RCFClientImpl();

    /**
    * @brief ~RCFClientImpl 析构函数
    */
    ~RCFClientImpl();

    /**
    * @brief init 初始化RCF客户端
    *
    * @param ip 服务器ip地址
    * @param port 服务器端口号，默认为50001
    */
    void init(const std::string& ip, unsigned int port = 50001);

    /**
    * @brief start 开启客户端服务
    *
    * @return 成功返回true，否则返回false
    */
    bool start();

    /**
    * @brief stop 停止客户端服务
    *
    * @return 成功返回true，否则返回false
    */
    bool stop();

    /**
    * @brief deinit 反初始化，释放一些资源
    */
    void deinit();

private:
    typedef boost::shared_ptr<RCF::RcfInitDeinit> RcfInitDeinitPtr;
    RcfInitDeinitPtr        m_rcfInit;                  ///< RCF客户端服务初始化对象

    typedef (boost::shared_ptr<RcfClient<RCFMessageHandler> >) RcfClientPtr;
    RcfClientPtr            m_rcfClient;                ///< RCF客户端对象

    std::string             m_ip;                       ///< 服务器ip地址
    unsigned int            m_port;                     ///< 服务器端口号

};

#endif
