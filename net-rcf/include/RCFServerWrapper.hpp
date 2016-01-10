/* Copyright(C)
* For free
* All right reserved
* 
*/
/**
* @file RCFServerWrapper.hpp
* @brief RCF服务端通信框架包装类
* @author highway-9, 787280310@qq.com
* @version 1.1.0
* @date 2016-01-04
*/

#ifndef _RCFSERVERWRAPPER_H
#define _RCFSERVERWRAPPER_H

#include "RCFServerImpl.hpp"

/**
* @brief RCF服务端通信框架包装类
*
* @tparam I_RCFMessageHander 类类型
*/
template<typename I_RCFMessageHander>
class RCFServerWrapper
{
public:
    /**
    * @brief RCFServerWrapper 构造函数
    */
    RCFServerWrapper(unsigned int port)
    {
        m_impl.reset();
        if (m_impl == NULL)
        {
            m_impl = boost::make_shared<RCFServerImpl<I_RCFMessageHander> >(port);
        }
    }

    /**
    * @brief ~RCFServerWrapper 析构函数
    */
    ~RCFServerWrapper()
    {
        // Do nothing
    }

    /**
    * @brief start 开启服务器
    *
    * @tparam RCFMessageHandler 类类型
    * @param rcfMessageHandler 消息处理对象
    *
    * @return 成功返回true，否则返回false
    */
    template<typename RCFMessageHandler>
    bool start(RCFMessageHandler& rcfMessageHandler)
    { 
        assert(m_impl != NULL);
        return m_impl->start(rcfMessageHandler);
    }

    /**
    * @brief stop 停止服务器
    *
    * @return 成功返回true，否则返回false
    */
    bool stop()
    {
        assert(m_impl != NULL);
        return m_impl->stop();
    }

private:
    typedef boost::shared_ptr<RCFServerImpl<I_RCFMessageHander> > RCFServerImplPtr;
    RCFServerImplPtr           m_impl;     ///< RCF服务器实现类指针
};

#endif
