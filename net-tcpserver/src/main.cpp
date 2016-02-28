/*************************************************************************
	> File Name: main.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年02月03日 星期三 15时06分27秒
 ************************************************************************/

#include <iostream>
#include "TcpServerImpl.h"
#include "PeopleInfoMessage.h"

class TcpServerMessageHandler
{
public:
    void handleReciveMessage(MessagePtr message)
    {
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
            boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));
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
};

int main()
{
    TcpServerImpl server(8888);
    server.setThreadPoolNum(10);

    TcpServerMessageHandler handler;

    ServerParam param;
    param.m_onRecivedMessage =
            boost::bind(&TcpServerMessageHandler::handleReciveMessage, &handler, _1);
    param.m_onHandleError =
            boost::bind(&TcpServerMessageHandler::handleError, &handler, _1, _2);
    param.m_onClientConnect =
            boost::bind(&TcpServerMessageHandler::handleClientConnect, &handler, _1);
    param.m_onClientDisconnect =
            boost::bind(&TcpServerMessageHandler::handleClientDisconnect, &handler, _1);
    server.setServerParam(param);

    server.start();

    std::cout << "Server starting..." << std::endl;

    boost::this_thread::sleep_for(boost::chrono::milliseconds(5000));

#if 0
    for (auto& iter : server.allRemoteAddress())
    {
        boost::shared_ptr<PeopleInfoMessage> peopleInfoMessage(new PeopleInfoMessage);
        peopleInfoMessage->m_messageType = 1000;
        peopleInfoMessage->m_name = "Jack";
        peopleInfoMessage->m_age = 20;
        server.asyncWrite(peopleInfoMessage, iter);

        boost::this_thread::sleep_for(boost::chrono::milliseconds(10));

        boost::shared_ptr<PeopleInfoMessage> peopleInfoMessage2(new PeopleInfoMessage);
        peopleInfoMessage2->m_messageType = 2000;
        peopleInfoMessage2->m_name = "Tom";
        peopleInfoMessage2->m_age = 22;
        server.asyncWrite(peopleInfoMessage2, iter);
    }
#endif

    boost::this_thread::sleep_for(boost::chrono::milliseconds(5000));

    std::cout << "Server stoped..." << std::endl;

    return 0;
}
