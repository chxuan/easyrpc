/*************************************************************************
	> File Name: main.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年02月03日 星期三 15时06分27秒
 ************************************************************************/

#include <iostream>
#include "TcpServerWrapper.h"
#include "PeopleInfoMessage.h"

class TcpServerMessageHandler
{
public:
    TcpServerMessageHandler(TcpServerWrapperPtr server)
    {
        assert(server.use_count() != 0);
        m_server = server;
    }

public:
    void handleReciveMessage(MessagePtr message, const std::string& remoteAddress)
    {
        std::cout << "remote address: " << remoteAddress << std::endl;
        unsigned int messageType = message->m_messageType;
        switch (messageType)
        {
        case 1000:
        {
            PeopleInfoMessage peopleInfoMessage;
            deSerialize(message, peopleInfoMessage);

            std::cout << "#################" << std::endl;
            std::cout << "m_messageType: " << peopleInfoMessage.m_messageType << std::endl;
            std::cout << "m_name: " << peopleInfoMessage.m_name << std::endl;
            std::cout << "m_age: " << peopleInfoMessage.m_age << std::endl;
            std::cout << "#################" << std::endl;

            PeopleInfoMessagePtr peopleInfo(new PeopleInfoMessage);
            peopleInfo->m_messageType = 1000;
            peopleInfo->m_name = "Jack";
            peopleInfo->m_age = 20;
            m_server->write(peopleInfo, remoteAddress);

            break;
        }
        default:
        {
            std::cout << "Unknown message type: " << messageType << std::endl;
            break;
        }
        }
    }

    void handleError(const boost::system::error_code& error, const std::string& remoteAddress)
    {
        if (error)
        {
            std::cout << "Tcp server handle error: " << error.message() << std::endl;
        }
    }

    void handleClientConnect(const std::string& remoteAddress)
    {
        std::cout << "Client connected, remote address: " << remoteAddress << std::endl;
    }

    void handleClientDisconnect(const std::string& remoteAddress)
    {
        std::cout << "Client disconnected, remote address: " << remoteAddress << std::endl;
    }

private:
    template<typename T>
    void deSerialize(const MessagePtr message, T& t)
    {
        std::istringstream is(message->m_data);
        boost::archive::binary_iarchive ia(is);
        ia >> t;
        t.m_messageType = message->m_messageType;
    }

private:
    TcpServerWrapperPtr m_server;
};

typedef boost::shared_ptr<TcpServerMessageHandler> TcpServerMessageHandlerPtr;

int main()
{
    TcpServerWrapperPtr server(new TcpServerWrapper(8888));
    server->setThreadPoolNum(10);

    TcpServerMessageHandlerPtr handler(new TcpServerMessageHandler(server));

    ServerParam param;
    param.m_onRecivedMessage =
            boost::bind(&TcpServerMessageHandler::handleReciveMessage, handler, _1, _2);
    param.m_onHandleError =
            boost::bind(&TcpServerMessageHandler::handleError, handler, _1, _2);
    param.m_onClientConnect =
            boost::bind(&TcpServerMessageHandler::handleClientConnect, handler, _1);
    param.m_onClientDisconnect =
            boost::bind(&TcpServerMessageHandler::handleClientDisconnect, handler, _1);
    server->setServerParam(param);

    server->start();

    std::cout << "Server starting..." << std::endl;

    std::cin.get();

    std::cout << "Server stoped..." << std::endl;

    return 0;
}
