/*************************************************************************
	> File Name: main.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年01月20日 星期三 17时08分08秒
 ************************************************************************/

#include <iostream>
#include "PubSubProtocolDefine.h"
#include "RCFSubscriberWrapper.hpp"

class RCFMessageHandler
{
public:
    void pushWeather(const WeatherInfoMessage& weatherInfo)
    {
        std::cout << weatherInfo.m_weatherDescription << std::endl;
    }
};

int main()
{
    RCFSubscriberWrapper<I_PubSubMessageHandler> server;
    RCFMessageHandler rcfMessageHandler;

    bool ok = server.start();
    if (ok)
    {
        std::cout << "Subscriber server start..." << std::endl;
    }
    else
    {
        std::cout << "Subscriber server start failed" << std::endl;
        return -1;
    }

    std::string ip = "127.0.0.1";
    unsigned int port = 50003;
    std::string topicName = "weather";
    ok = server.createSubscriber(rcfMessageHandler, SubscriberParam(ip, port, topicName));
    if (ok)
    {
        std::cout << "Subscription topic success, topic name: " << topicName << std::endl;
    }
    else
    {
        std::cout << "Subscription topic failed, topic name: " << topicName << std::endl;
        return -1;
    }

    std::cin.get();

    std::cout << "Subscriber server stoped..." << std::endl;

    return 0;
}
