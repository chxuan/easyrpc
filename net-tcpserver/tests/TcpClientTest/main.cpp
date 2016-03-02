/*************************************************************************
	> File Name: main.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年02月03日 星期三 15时06分27秒
 ************************************************************************/

#include <iostream>
#include "TcpClientWrapper.h"
#include "PeopleInfoMessage.h"

class TcpClientMessageHandler
{
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
            std::cout << "Tcp client handle error: " << error.message() << std::endl;
        }
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

typedef boost::shared_ptr<TcpClientMessageHandler> TcpClientMessageHandlerPtr;

int main()
{
    TcpClientWrapperPtr client(new TcpClientWrapper("127.0.0.1", 8888));
    client->setThreadPoolNum(10);

    TcpClientMessageHandlerPtr handler(new TcpClientMessageHandler);

    ClientParam param;
    param.m_onRecivedMessage =
            boost::bind(&TcpClientMessageHandler::handleReciveMessage, handler, _1, _2);
    param.m_onHandleError =
            boost::bind(&TcpClientMessageHandler::handleError, handler, _1, _2);
    client->setClientParam(param);

    client->start();

    std::cout << "Client starting..." << std::endl;

    for (int i = 0; i < 10; ++i)
    {
        PeopleInfoMessagePtr peopleInfoMessage(new PeopleInfoMessage);
        peopleInfoMessage->m_messageType = 1000;
        peopleInfoMessage->m_name = "Jack";
        peopleInfoMessage->m_age = 20;
        client->write(peopleInfoMessage);
    }

    boost::this_thread::sleep_for(boost::chrono::milliseconds(5000));

    std::cout << "Client stoped..." << std::endl;

    return 0;
}
