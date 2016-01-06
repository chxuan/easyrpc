/* Copyright(C)
* For free
* All right reserved
* 
*/
/**
* @file RCFServerImpl.cpp
* @brief RCF服务端通信框架实现
* @author highway-9, 787280310@qq.com
* @version 1.1.0
* @date 2016-01-04
*/

#include "RCFServerImpl.h"

RCFServerImpl::RCFServerImpl()
    : m_rcfInit(NULL),
    m_rcfServer(NULL),
    m_port(50001)
{
    // Do nothing
}

RCFServerImpl::~RCFServerImpl()
{
    stop();
    deinit();
}

void RCFServerImpl::init(unsigned int port)
{
    m_port = port;
}

bool RCFServerImpl::start()
{
    try
    {
        if (m_rcfInit == NULL)
        {
            m_rcfInit = std::make_shared<RCF::RcfInitDeinit>();
        }


    }
    catch ()
    {

    }

    return true;
}

bool RCFServerImpl::stop()
{
    return true;
}

void RCFServerImpl::deinit()
{
    
}
