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

/**
* @brief RCF服务端通信框架实现类
*
* @tparam T 类类型
*/
template<typename T>
class RCFServerImpl
{
public:
    /**
    * @brief RCFServerImpl 构造函数
    */
    RCFServerImpl();

    /**
    * @brief ~RCFServerImpl 析构函数
    */
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
    * @brief setMessageHandler 设置消息处理类
    *
    * @tparam T2 类类型
    * @param rcfMessageHandler 消息处理对象
    */
    template<typename T2>
    void setMessageHandler(T2* rcfMessageHandler)
    {
        if (m_rcfMessageHandler != NULL) 
        {
            m_rcfMessageHandler = rcfMessageHandler;
        }
    }

private:
    typedef boost::shared_ptr<RCF::RcfInitDeinit> RcfInitDeinitPtr;
    RcfInitDeinitPtr        m_rcfInit;                  ///< RCF服务器初始化对象

    typedef boost::shared_ptr<RCF::RcfServer> RcfServerPtr;
    RcfServerPtr            m_rcfServer;                ///< RCF服务器对象

    T2*                     m_rcfMessageHandler;        ///< RCF消息处理对象
    unsigned int            m_port;                     ///< 监听端口
};

#endif
