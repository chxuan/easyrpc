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
#include <iostream>
#include <unordered_map>
#include <boost/smart_ptr.hpp>

typedef boost::shared_ptr<RCF::RcfInitDeinit>                   RcfInitDeinitPtr;
typedef boost::shared_ptr<RCF::RcfServer>                       RcfServerPtr;
typedef boost::shared_ptr<RCF::Publisher<I_RCFMessageHandler> > RcfPublisherPtr;

template<typename I_RCFMessageHandler>
class RCFPublisherImpl
{
public:
    RCFPublisherImpl(unsigned int port)
        : m_port(port)
    {
        m_rcfInit.reset();
        m_rcfServer.reset();
        m_rcfPublisher.reset();
    }

    ~RCFPublisherImpl()
    {
        stop();       
    }

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

    bool createPublisher(const std::string& topicName)
    {
        if (isPublisherExists(topicName))
        {
            return false;
        }

        try
        {
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

    bool stop()
    {
        bool ok = closeAllPublisher();
        if (!ok)
        {
            return false;
        }

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

private:
    bool isPublisherExists(const std::string& topicName)
    {
        RcfPublisherMap::const_iterator iter = m_rcfPublisherMap.find(topicName);
        return (iter != m_rcfPublisherMap.end()) ? true : false;
    }

    bool closePublisher(const std::string& topicName)
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

    bool closeAllPublisher()
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

private:
    RcfInitDeinitPtr        m_rcfInit;                  ///< RCF服务器初始化对象
    RcfServerPtr            m_rcfServer;                ///< RCF服务器对象
    unsigned int            m_port;                     ///< 发布的端口号

    typedef std::unordered_map<std::string, RcfPublisherPtr> RcfPublisherMap;
    RcfPublisherMap         m_rcfPublisherMap;          ///< 发布者map，key：主题名，value：发布者
};

#endif
