/*************************************************************************
	> File Name: main.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年01月17日 星期日 09时25分19秒
 ************************************************************************/

#include <iostream>
#include <string>
#include "PubSubProtocolDefine.h"
#include "RCFPublisherWrapper.hpp"

int main()
{
    RCFPublisherWrapper<I_PubSubMessageHandler> server(50003);
    server.start();

    std::string topicName = "weather";
    bool ok = server.createPublisher(topicName);
    if (ok)
    {
        std::cout << "Create publisher success, topicName: " << topicName << std::endl;
    }
    else
    {
        std::cout << "Create publisher failed, topicName: " << topicName << std::endl;
    }

    return 0;
}
