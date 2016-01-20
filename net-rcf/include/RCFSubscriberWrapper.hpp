/* Copyright(C)
* For free
* All right reserved
* 
*/
/**
* @file RCFSubscriberWrapper.hpp
* @brief 订阅者包装类
* @author highway-9, 787280310@qq.com
* @version 1.1.0
* @date 2016-01-12
*/

#ifndef _RCFSUBSCRIBERWRAPPER_H
#define _RCFSUBSCRIBERWRAPPER_H

#include "RCFSubscriberImpl.hpp"

/**
* @brief 订阅者包装类
*
* @tparam I_RCFMessageHandler 类类型
*/
template<typename I_RCFMessageHandler>
class RCFSubscriberWrapper
{
public:
    /**
    * @brief RCFSubscriberWrapper 构造函数
    */
    RCFSubscriberWrapper()
    {
        m_impl.reset();
        if (m_impl == NULL)
        {
            m_impl = boost::make_shared<RCFSubscriberImpl<I_RCFMessageHandler> >();
        }
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
        assert(m_impl != NULL);
        return m_impl->start();
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
        assert(m_impl != NULL);
        return m_impl->createSubscriber(rcfMessageHandler, param);
    }

    /**
    * @brief stop 停止订阅者服务器
    *
    * @return 成功返回true，否则返回false
    */
    bool stop()
    {
        assert(m_impl != NULL);
        return m_impl->stop();
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
        assert(m_impl != NULL);
        return m_impl->closeSubscriber(topicName);
    }

    /**
    * @brief closeAllSubscriber 停止所有的订阅者
    *
    * @return 成功返回true，否则返回false
    */
    bool closeAllSubscriber()
    {
        assert(m_impl != NULL);
        return m_impl->closeAllSubscriber();
    }

private:
    typedef typename boost::shared_ptr<RCFSubscriberImpl<I_RCFMessageHandler> > RCFSubscriberImplPtr;
    RCFSubscriberImplPtr             m_impl;             ///< RCF订阅者实现对象
};

#endif
