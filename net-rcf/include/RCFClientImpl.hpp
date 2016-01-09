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
#include <iostream>
#include <boost/shared_ptr.hpp>

/**
* @brief RCF客户端通信框架实现类
*
* @tparam T 类类型
*/
template<typename T>
class RCFClientImpl
{
public:
    /**
    * @brief RCFClientImpl 构造函数
    */
    RCFClientImpl()
        : m_rcfInit(NULL),
        m_rcfClient(NULL),
        m_ip(127.0.0.1),
        m_port(50001)
    {
        // Do nothing
    }

    /**
    * @brief ~RCFClientImpl 析构函数
    */
    ~RCFClientImpl()
    {
        stop();
        deinit();
    }

    /**
    * @brief init 初始化RCF客户端
    *
    * @param ip 服务器ip地址
    * @param port 服务器端口号，默认为50001
    */
    void init(const std::string& ip, unsigned int port = 50001)
    {
        m_ip = ip;
        m_port = port;
    }

    /**
    * @brief start 开启客户端服务
    *
    * @return 成功返回true，否则返回false
    */
    bool start()
    {
        try
        {
            if (m_rcfInit == NULL)
            {
                m_rcfInit = boost::make_shared<RCF::RcfInitDeinit>();
            }

            if (m_rcfClient == NULL)
            {
                m_rcfClient = boost::make_shared<RcfClient<T> >(RCF::TcpEndPoint(m_ip, m_port));
            }
        }
        catch (const RCF::Exception& e)
        {
            std::cout << "Error: " << e.getErrorString() << std::endl;
            return false;
        }

        return true;
    }

    /**
    * @brief stop 停止客户端服务
    *
    * @return 成功返回true，否则返回false
    */
    bool stop()
    {
        return true;
    }

    /**
    * @brief deinit 反初始化，释放一些资源
    */
    void deinit()
    {
        // Do nothing
    }

private:
    typedef boost::shared_ptr<RCF::RcfInitDeinit> RcfInitDeinitPtr;
    RcfInitDeinitPtr        m_rcfInit;                  ///< RCF客户端服务初始化对象

    typedef boost::shared_ptr<RcfClient<T> > RcfClientPtr;
    RcfClientPtr            m_rcfClient;                ///< RCF客户端对象

    std::string             m_ip;                       ///< 服务器ip地址
    unsigned int            m_port;                     ///< 服务器端口号
};

#endif
