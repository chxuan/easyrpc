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

#include "RCFClientImpl.hpp"

/**
* @brief RCf客户端通信框架包装类
*
* @tparam I_RCFMessageHandler 类类型
*/
template<typename I_RCFMessageHandler>
class RCFClientWrapper
{
public:
    /**
    * @brief RCFClientWrapper 构造函数
    */
    RCFClientWrapper()
        : m_impl(NULL)
    {
        m_impl = boost::make_shared<RCFClientImpl<I_RCFMessageHandler> >();
    }

    /**
    * @brief ~RCFClientWrapper 析构函数
    */
    ~RCFClientWrapper()
    {
        // Do nothing
    }

    /**
    * @brief init 初始化RCF客户端
    *
    * @param ip 服务器ip地址
    * @param port 服务器端口号，默认为50001
    */
    void init(const std::string& ip, unsigned int port = 50001)
    {
        if (m_impl != NULL)
        {
            m_impl->init(ip, port);
        }
    }

    /**
    * @brief deinit 反初始化，释放一些资源
    */
    void deinit()
    {
        if (m_impl != NULL)
        {
            m_impl->deinit();
        }
    }

private:
    typedef boost::shared_ptr<RCFClientImpl<I_RCFMessageHandler> > RCFClientImplPtr;
    RCFClientImplPtr        m_impl;		///< RCF客户端实现类指针
};

#endif
