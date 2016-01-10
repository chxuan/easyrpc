/*************************************************************************
	> File Name: CenterNetServer.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年01月10日 星期日 20时38分13秒
 ************************************************************************/

#include "CenterNetServer.h"
#include <assert.h>

RCFMessageHandler::RCFMessageHandler(CenterNetServer* centerNetServer)
{
    assert(centerNetServer != NULL);
    m_centerNetServer = centerNetServer;
}

RCFMessageHandler::~RCFMessageHandler()
{
    // Do nothing
}

bool RCFMessageHandler::queryPeopleInfoByID(int id, PeopleInfoMessage& peopleInfo)
{
    try
    {
        return m_centerNetServer->m_rcfDBServerClientWrapper->rcfClientObject()->queryPeopleInfoByID(id, peopleInfo);
    }
    catch (const RCF::Exception& e)
    {
        std::cout << "Error: " << e.getErrorString() << std::endl;
        return false;
    }

    return false;
}

CenterNetServer::CenterNetServer()
{
    m_rcfServerWrapper.reset();
    m_rcfDBServerClientWrapper.reset();
    m_rcfMessageHandler.reset();
}

CenterNetServer::~CenterNetServer()
{
    // Do nothing
}

bool CenterNetServer::start()
{
    if (m_rcfServerWrapper == NULL)
    {
        m_rcfServerWrapper = boost::make_shared<RCFServerWrapper<I_RCFMessageHandler> >(50001);

        if (m_rcfMessageHandler == NULL)
        {
            m_rcfMessageHandler = boost::make_shared<RCFMessageHandler>(this);
            bool ok = m_rcfServerWrapper->start(*m_rcfMessageHandler);
            if (!ok)
            {
                return false;
            }
        }
    }

    if (m_rcfDBServerClientWrapper == NULL)
    {
        m_rcfDBServerClientWrapper = boost::make_shared<RCFClientWrapper<I_RCFMessageHandler> >("127.0.0.1", 50002);
    }

    return true;
}

bool CenterNetServer::stop()
{
    return m_rcfServerWrapper->stop();    
}
