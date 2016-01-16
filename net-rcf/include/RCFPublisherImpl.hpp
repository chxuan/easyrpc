/* Copyright(C)
* For free
* All right reserved
* 
*/
/**
* @file RCFPublisherImpl.hpp
* @brief 发布者实现类
* @author highway-9, 787280310@qq.com
* @version 1.1.0
* @date 2016-01-12
*/

#ifndef _RCFPUBLISHERIMPL_H
#define _RCFPUBLISHERIMPL_H

#include <RCF/RCF.hpp>
#include <assert.h>
#include <iostream>
#include <unordered_map>
#include <boost/smart_ptr.hpp>

typedef boost::shared_ptr<RCF::RcfInitDeinit>                   RcfInitDeinitPtr;
typedef boost::shared_ptr<RCF::RcfServer>                       RcfServerPtr;
typedef boost::shared_ptr<RCF::Publisher<I_RCFMessageHandler> > RcfPublisherPtr;

/**
* @brief 发布者实现类
*
* @tparam I_RCFMessageHandler 类类型
*/
template<typename I_RCFMessageHandler>
class RCFPublisherImpl
{
public:
    /**
    * @brief RCFPublisherImpl 构造函数
    *
    * @param port 发布的端口号
    */
    RCFPublisherImpl(unsigned int port);

    /**
    * @brief ~RCFPublisherImpl 析构函数
    */
    ~RCFPublisherImpl();

    /**
    * @brief start 开启服务器
    *
    * @note 开启服务器之后，才能调用createPublisher函数
    *
    * @return 成功返回true，否则返回false
    */
    bool start();

    /**
    * @brief createPublisher 通过主题来创建发布者
    *
    * @param topicName 主题名称
    *
    * @note 调用该函数之前，请先调用start函数开启服务器
    *
    * @return 成功返回true，否则返回false
    */
    bool createPublisher(const std::string& topicName);

    /**
    * @brief stop 停止发布者服务器
    *
    * @return 成功返回true，否则返回false
    */
    bool stop();

private:
    /**
    * @brief isPublisherExists 判断主题是否存在
    *
    * @param topicName 主题名称
    *
    * @return 存在返回true，否则返回false
    */
    bool isPublisherExists(const std::string& topicName);

    /**
    * @brief closePublisher 通过主题来停止发布者
    *
    * @param topicName 主题名称
    *
    * @return 成功返回true，否则返回false
    */
    bool closePublisher(const std::string& topicName);

    /**
    * @brief closeAllPublisher 停止所有的发布者
    *
    * @return 成功返回true，否则返回false
    */
    bool closeAllPublisher();

private:
    RcfInitDeinitPtr        m_rcfInit;                  ///< RCF服务器初始化对象
    RcfServerPtr            m_rcfServer;                ///< RCF服务器对象
    unsigned int            m_port;                     ///< 发布的端口号

    typedef std::unordered_map<std::string, RcfPublisherPtr> RcfPublisherMap;
    RcfPublisherMap         m_rcfPublisherMap;          ///< 发布者map，key：主题名，value：发布者
};

template<typename I_RCFMessageHandler>
RCFPublisherImpl<I_RCFMessageHandler>::RCFPublisherImpl(unsigned int port)
{
    m_rcfInit.reset();
    m_rcfServer.reset();
}

template<typename I_RCFMessageHandler>
RCFPublisherImpl<I_RCFMessageHandler>::~RCFPublisherImpl()
{
    stop();
}

template<typename I_RCFMessageHandler>
bool RCFPublisherImpl<I_RCFMessageHandler>::start()
{
    try
    {
        if (m_rcfInit == NULL)
        {
            m_rcfInit = boost::make_shared<RCF::RcfInitDeinit>();
        }

        if (m_rcfServer == NULL)
        {
            m_rcfServer = boost::make_shared<RCF::RcfServer>(RCF::TcpEndpoint(m_port));
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
bool RCFPublisherImpl<I_RCFMessageHandler>::createPublisher(const std::string& topicName)
{
    if (isPublisherExists(topicName))
    {
        return false;
    }

    try
    {
        assert(m_rcfServer != NULL);
        RCF::PublisherParms pubParms;
        pubParms.setTopicName(topicName);
        RcfPublisherPtr rcfPublisher = m_rcfServer->createPublisher<I_RCFMessageHandler>(pubParms);
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
bool RCFPublisherImpl<I_RCFMessageHandler>::stop()
{
    bool ok = closeAllPublisher();
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
bool RCFPublisherImpl<I_RCFMessageHandler>::isPublisherExists(const std::string& topicName)
{
    RcfPublisherMap::const_iterator iter = m_rcfPublisherMap.find(topicName);
    if (iter != m_rcfPublisherMap.end())
    {
        return true;
    }

    return false;
}

template<typename I_RCFMessageHandler>
bool RCFPublisherImpl::<I_RCFMessageHandler>::closePublisher(const std::string& topicName)
{
    RcfPublisherMap::const_iterator iter = m_rcfPublisherMap.find(topicName);
    if (iter != m_rcfPublisherMap.end())
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

        m_rcfPublisherMap.erase(iter);

        return true;
    }

    return false;
}

template<typename I_RCFMessageHandler>
bool RCFPublisherImpl<I_RCFMessageHandler>::closeAllPublisher()
{
    RcfPublisherMap::const_iterator begin = m_rcfPublisherMap.begin();
    RcfPublisherMap::const_iterator end = m_rcfPublisherMap.end();

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

    m_rcfPublisherMap.clear();
}

#endif
