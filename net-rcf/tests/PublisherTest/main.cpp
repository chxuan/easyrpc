/*************************************************************************
	> File Name: main.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年01月17日 星期日 09时25分19秒
 ************************************************************************/

#include <iostream>
#include "PubSubProtocolDefine.h"
#include "RCFPublisherWrapper.hpp"

int main()
{
    RCFPublisherWrapper<I_PubSubMessageHandler> server(50003);
    server.start();

    RcfPublisherPtr publisher = server.createPublisher("weather");
    assert(publisher != NULL);

    return 0;
}
