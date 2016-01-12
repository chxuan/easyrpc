/* Copyright(C)
* For free
* All right reserved
* 
*/
/**
* @file RCFPublisherWrapper.hpp
* @brief 发布者包装类
* @author highway-9, 787280310@qq.com
* @version 1.1.0
* @date 2016-01-12
*/

#include "RCFPublisherImpl.hpp"

template<typename I_RCFMessageHandler>
class RCFPublisherWrapper
{
public:
    RCFPublisherWrapper()
    {
        m_impl.reset();
        if (m_impl == NULL)
        {
            m_impl = boost::make_shared<RCFPublisherImpl<I_RCFMessageHandler> >();
        }
    }

    ~RCFPublisherWrapper()
    {
        // Do nothing
    }

private:
    typedef boost::shared_ptr<RCFPublisherImpl<I_RCFMessageHandler> > RCFPublisherImplPtr;
    RCFPublisherImplPtr             m_impl;
};
