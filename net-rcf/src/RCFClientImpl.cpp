/* Copyright(C)
* For free
* All right reserved
* 
*/
/**
* @file RCFClientImpl.cpp
* @brief RCF客户端通信框架实现
* @author highway-9, 787280310@qq.com
* @version 1.1.0
* @date 2016-01-04
*/

#include "RCFClientImpl.h"

RCFClientImpl::RCFClientImpl()
    : m_rcfInit(NULL),
    m_rcfClient(NULL),
    m_ip(127.0.0.1),
    m_port(50001)
{
    // Do nothing
}

RCFClientImpl::~RCFClientImpl()
{
    stop();
    deinit();
}

void RCFClientImpl::init(const std::string& ip, unsigned int port)
{
    m_ip = ip;
    m_port = port;
}

bool RCFClientImpl::start()
{
    try
    {
        if (m_rcfInit == NULL)
        {
            m_rcfInit = boost::make_shared<RCF::RcfInitDeinit>();
        }

        if (m_rcfClient == NULL)
        {
            m_rcfClient = boost::make_shared<RcfClient<RCFMessageHandler> >(RCF::TcpEndPoint(m_ip, m_port));
        }
    }
    catch (const RCF::Exception& e)
    {
        std::cout << "Error: " << e.getErrorString() << std::endl;
        return false;
    }

    return true;
}

bool RCFClientImpl::stop()
{
    return true;
}

void RCFClientImpl::deinit()
{
   // Do nothing 
}

