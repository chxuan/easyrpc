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

typedef boost::shared_ptr<RCF::RcfInitDeinit>                   RcfInitDeinitPtr;
typedef boost::shared_ptr<RCF::RcfServer>                       RcfServerPtr;

class SubscriberParam
{
public:
    SubscriberParam(const std::string& ip, unsigned int port, const std::string& topicName)
        : m_ip(ip),
        m_port(port),
        m_topicName(topicName)
    {
        // Do nothing
    }

    std::string         m_ip;
    unsigned int        m_port;
    std::string         m_topicName;
};

template<typename I_RCFMessageHandler>
class RCFSubscriberImpl
{
public:
    RCFSubscriberImpl();

    ~RCFSubscriberImpl();

    bool start();

    template<typename RCFMessageHandler>
    bool createSubscriber(RCFMessageHandler& rcfMessageHandler, const SubscriberParam& param);

    bool stop();

private:
    /**
    * @brief isSubscriberExists 判断订阅者是否存在
    *
    * @param topicName 主题名称
    *
    * @return 存在返回true，否则返回false
    */
    bool isSubscriberExists(const std::string& topicName);

    /**
    * @brief closeSubscriber 通过主题来停止订阅者
    *
    * @param topicName 主题名称
    *
    * @return 成功返回true，否则返回false
    */
    bool closeSubscriber(const std::string& topicName);

    /**
    * @brief closeAllSubscriber 停止所有的订阅者
    *
    * @return 成功返回true，否则返回false
    */
    bool closeAllSubscriber();

private:
    RcfInitDeinitPtr        m_rcfInit;                  ///< RCF服务器初始化对象
    RcfServerPtr            m_rcfServer;                ///< RCF服务器对象

    typedef std::unordered_map<std::string, RCF::SubscriptionPtr> RcfSubscriberMap;
    RcfSubscriberMap        m_rcfSubsriberMap;          ///< 订阅者map，key：主题名，value：订阅者
};

template<typename I_RCFMessageHandler>
RCFSubscriberImpl<I_RCFMessageHandler>::RCFSubscriberImpl()
{
    m_rcfInit.reset();
    m_rcfServer.reset();
}

template<typename I_RCFMessageHandler>
RCFSubscriberImpl<I_RCFMessageHandler>::~RCFSubscriberImpl()
{
    stop();
}

template<typename I_RCFMessageHandler>
bool RCFSubscriberImpl<I_RCFMessageHandler>::start()
{
    try
    {
        if (m_rcfInit == NULL)
        {
            m_rcfInit = boost::make_shared<RCF::RcfInitDeinit>();
        }

        if (m_rcfServer == NULL)
        {
            unsigned int port = -1;
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

template<typename I_RCFMessageHandler>
bool RCFSubscriberImpl<I_RCFMessageHandler>::stop()
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

template<typename I_RCFMessageHandler>
template<typename RCFMessageHandler>
bool RCFSubscriberImpl<I_RCFMessageHandler>::createSubscriber(RCFMessageHandler& rcfMessageHandler,
                                                              const SubscriberParam& param)
{
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

template<typename I_RCFMessageHandler>
bool RCFSubscriberImpl<I_RCFMessageHandler>::isSubscriberExists(const std::string& topicName)
{
    RcfSubscriberMap::const_iterator iter = m_rcfSubsriberMap.find(topicName);
    if (iter != m_rcfPublisherMap.end())
    {
        return true;
    }

    return false;
}

template<typename I_RCFMessageHandler>
bool RCFSubscriberImpl::<I_RCFMessageHandler>::closeSubscriber(const std::string& topicName)
{
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

template<typename I_RCFMessageHandler>
bool RCFSubscriberImpl<I_RCFMessageHandler>::closeAllSubscriber()
{
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

#endif
