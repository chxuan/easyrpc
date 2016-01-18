/* Copyright(C)
* For free
* All right reserved
* 
*/
/**
* @file RCFSubscriberImpl.hpp
* @brief 订阅者实现类
* @author highway-9, 787280310@qq.com
* @version 1.1.0
* @date 2016-01-12
*/

#ifndef _RCFSUBSCRIBERIMPL_H
#define _RCFSUBSCRIBERIMPL_H

#include <RCF/RCF.hpp>
#include <assert.h>
#include <iostream>
#include <unordered_map>
#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>

/**
* @brief 订阅者参数
*/
class SubscriberParam
{
public:
    /**
    * @brief SubscriberParam 构造函数
    *
    * @param ip 发布者的IP地址
    * @param port 发布者的端口号
    * @param topicName 订阅的主题
    */
    SubscriberParam(const std::string& ip, unsigned int port, const std::string& topicName)
        : m_ip(ip),
        m_port(port),
        m_topicName(topicName)
    {
        // Do nothing
    }

    std::string         m_ip;               ///< 发布者的IP地址
    unsigned int        m_port;             ///< 发布者的端口号
    std::string         m_topicName;        ///< 订阅的主题 
};

/**
* @brief 订阅者实现类
*
* @tparam I_RCFMessageHandler 类类型
*/
template<typename I_RCFMessageHandler>
class RCFSubscriberImpl
{
public:
    /**
    * @brief RCFSubscriberImpl 构造函数
    */
    RCFSubscriberImpl()
    {
        m_rcfInit.reset();
        m_rcfServer.reset();
    }

    /**
    * @brief ~RCFSubscriberImpl 析构函数
    */
    ~RCFSubscriberImpl()
    {
        stop();
    }

    /**
    * @brief start 开启服务器
    *
    * @note 开启服务器之后，才能调用createSubscriber函数
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

            if (m_rcfServer == NULL)
            {
                int port = -1;
                m_rcfServer = boost::make_shared<RCF::RcfServer>(RCF::TcpEndpoint(port));
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
    * @brief createSubscriber 创建订阅
    *
    * @tparam RCFMessageHandler 类类型
    * @param rcfMessageHandler 消息处理对象
    * @param param 订阅者参数
    *
    * @note 调用该函数之前，请先调用start函数开启服务器
    *
    * @return 成功返回true，否则返回false
    */
    template<typename RCFMessageHandler>
    bool createSubscriber(RCFMessageHandler& rcfMessageHandler, const SubscriberParam& param)
    {
        boost::lock_guard<boost::mutex> locker(m_mutex);

        if (isSubscriberExists(topicName))
        {
            return false;
        }

        try
        {
            assert(m_rcfServer != NULL);
            RCF::SubscriptionParms subParms;
            subParms.setPublisherEndpoint(RCF::TcpEndpoint(param.m_ip, param.m_port))
            subParms.setTopicName(param.m_topicName);
            RCF::SubscriptionPtr rcfSubscriber = m_rcfServer->createSubscription<I_RCFMessageHandler>(rcfMessageHandler, subParms);
        }
        catch (const RCF::Exception& e)
        {
            std::cout << "Error: " << e.getErrorString() << std::endl;
            return false;
        }

        m_rcfPublisherMap.insert(std::make_part(topicName, rcfPublisher));

        return true;
    }

    /**
    * @brief stop 停止订阅者服务器
    *
    * @return 成功返回true，否则返回false
    */
    bool stop()
    {
        bool ok = closeAllSubscriber();
        if (!ok)
        {
            return false;
        }

        try
        {
            assert(m_rcfServer != NULL);
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
    * @brief closeSubscriber 通过主题来停止订阅者
    *
    * @param topicName 主题名称
    *
    * @return 成功返回true，否则返回false
    */
    bool closeSubscriber(const std::string& topicName)
    {
        boost::lock_guard<boost::mutex> locker(m_mutex);

        RcfSubscriberMap::const_iterator iter = m_rcfSubsriberMap.find(topicName);
        if (iter != m_rcfSubsriberMap.end())
        {
            try
            {
                iter->second->close();
            }
            catch (const RCF::Exception& e)
            {
                std::cout << "Error: " << e.getErrorString() << std::endl;
                return false;
            }

            m_rcfSubsriberMap.erase(iter);

            return true;
        }

        return false;
    }

    /**
    * @brief closeAllSubscriber 停止所有的订阅者
    *
    * @return 成功返回true，否则返回false
    */
    bool closeAllSubscriber()
    {
        boost::lock_guard<boost::mutex> locker(m_mutex);

        RcfSubscriberMap::const_iterator begin = m_rcfSubsriberMap.begin();
        RcfSubscriberMap::const_iterator end = m_rcfSubsriberMap.end();

        while (begin != end)
        {
            try
            {
                begin->second->close();
            }
            catch (const RCF::Exception& e)
            {
                std::cout << "Error: " << e.getErrorString() << std::endl;
                return false;
            }

            ++begin;
        }

        m_rcfSubsriberMap.clear();
    }

private:
    /**
    * @brief isSubscriberExists 判断订阅者是否存在
    *
    * @param topicName 主题名称
    *
    * @return 存在返回true，否则返回false
    */
    bool isSubscriberExists(const std::string& topicName)
    {
        RcfSubscriberMap::const_iterator iter = m_rcfSubsriberMap.find(topicName);
        if (iter != m_rcfPublisherMap.end())
        {
            return true;
        }

        return false;
    }

private:
    typedef boost::shared_ptr<RCF::RcfInitDeinit> RcfInitDeinitPtr;
    RcfInitDeinitPtr        m_rcfInit;                  ///< RCF服务器初始化对象

    typedef boost::shared_ptr<RCF::RcfServer> RcfServerPtr;
    RcfServerPtr            m_rcfServer;                ///< RCF服务器对象

    typedef std::unordered_map<std::string, RCF::SubscriptionPtr> RcfSubscriberMap;
    RcfSubscriberMap        m_rcfSubsriberMap;          ///< 订阅者map，key：主题名，value：订阅者

    boost::mutex            m_mutex;                    ///< 订阅者map互斥锁
};

#endif
