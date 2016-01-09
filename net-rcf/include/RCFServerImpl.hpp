/* Copyright(C)
* For free
* All right reserved
* 
*/
/**
* @file RCFServerImpl.hpp
* @brief RCF服务端通信框架实现类
* @author highway-9, 787280310@qq.com
* @version 1.1.0
* @date 2016-01-04
*/

#ifndef _RCFSERVERIMPL_H
#define _RCFSERVERIMPL_H

#include <RCF/RCF.hpp>
#include <iostream>
#include <boost/shared_ptr.hpp>

/**
* @brief RCF服务端通信框架实现类
*
* @tparam I_RCFMessageHandler 类类型
*/
template<typename I_RCFMessageHandler>
class RCFServerImpl
{
public:
    /**
    * @brief RCFServerImpl 构造函数
    */
    RCFServerImpl()
        : m_rcfInit(NULL),
        m_rcfServer(NULL),
        m_rcfMessageHandler(NULL),
        m_port(50001)
    {
        // Do nothing
    }

    /**
    * @brief ~RCFServerImpl 析构函数
    */
    ~RCFServerImpl()
    {
        stop();
        deinit();
    }

    /**
    * @brief init 初始化RCF服务器端
    *
    * @param port 监听端口，默认为50001
    */
    void init(unsigned int port = 50001)
    {
        m_port = port;
    }

    /**
    * @brief start 开始服务器
    *
    * @return 成功返回true，否则返回false
    */
    bool start()
    {
        if (m_rcfMessageHandler == NULL)
        {
            std::cout << "RCF message hander is NULL" << std::endl;
            return false;
        }

        try
        {
            if (m_rcfInit == NULL)
            {
                m_rcfInit = boost::make_shared<RCF::RcfInitDeinit>();
            }

            if (m_rcfServer == NULL)
            {
                m_rcfServer = boost::make_shared<RCF::RcfServer>(RCF::TcpEndPoint(m_port));
                m_rcfServer->bind<I_RCFMessageHandler>(*m_rcfMessageHandler);

                RCF::ThreadPoolPtr threadPool(new RCF::ThreadPool(1, MAX_THREAD_NUM));
                m_rcfServer->setThreadPool(threadPool);

                m_rcfServer->start();
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
    * @brief stop 停止服务器
    *
    * @return 成功返回true，否则返回false
    */
    bool stop()
    {
        try
        {
            m_rcfServer->stop();    
        }
        catch (const RCF::Exception& e)
        {
            std::cout << "Error: " << e.getErrorString() << std::endl;
            return false;
        }

        return true;
    }

    /**
    * @brief deinit 反初始化，释放一些资源
    */
    void deinit()
    {
        // Do nothing    
    }

    /**
    * @brief setMessageHandler 设置消息处理类
    *
    * @tparam RCFMessageHandler 类类型
    * @param rcfMessageHandler 消息处理对象
    */
    template<typename RCFMessageHandler>
    void setMessageHandler(RCFMessageHandler* rcfMessageHandler)
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

    RCFMessageHandler*      m_rcfMessageHandler;        ///< RCF消息处理对象
    unsigned int            m_port;                     ///< 监听端口
};

#endif
