/*************************************************************************
	> File Name: main.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年02月03日 星期三 15时06分27秒
 ************************************************************************/

#include <iostream>
#include "TcpServerImpl.h"
#include "PeopleInfoMessage.h"

int main()
{
    std::cout << "Server starting..." << std::endl;
    TcpServerImpl server(8888);
    server.start();

    std::cout << "###############################" << std::endl;

    //std::cin.get();
    boost::this_thread::sleep_for(boost::chrono::milliseconds(5000));

    for (auto& iter : server.allRemoteAddress())
    {
        //PeopleInfoMessage *peopleInfoMessage = new PeopleInfoMessage;
        boost::shared_ptr<PeopleInfoMessage> peopleInfoMessage(new PeopleInfoMessage);
        peopleInfoMessage->m_messageType = 1000;
        peopleInfoMessage->m_name = "Jack";
        peopleInfoMessage->m_age = 20;
        server.asyncWrite(peopleInfoMessage, iter);
    }

    boost::this_thread::sleep_for(boost::chrono::milliseconds(5000));

    std::cout << "Server stoped..." << std::endl;

    return 0;
}
