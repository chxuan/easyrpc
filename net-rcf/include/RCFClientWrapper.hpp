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
    * @brief deinit 反初始化，释放一些资源
    */
    void deinit();

private:
    typedef boost::shared_ptr<RCFClientImpl> RCFClientImplPtr;
    RCFClientImplPtr        m_impl;		///< RCF客户端实现类指针
};

#endif
