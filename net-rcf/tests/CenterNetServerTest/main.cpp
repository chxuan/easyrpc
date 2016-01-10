/*************************************************************************
	> File Name: main.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年01月09日 星期六 20时26分44秒
 ************************************************************************/

#include <iostream>
#include "ProtolDef.h"
#include "RCFServerWrapper.hpp"

class RCFMessageHandler
{
public:
    void print(const std::string& str)
    {
        std::cout << str << std::endl;
    }
};

int main()
{
    RCFMessageHandler rcfMessageHandler;
    RCFServerWrapper<I_RCFMessageHandler> server(50001);
    server.start(rcfMessageHandler);

    std::cout << "Center net server start..." << std::endl;

    std::cin.get();

    server.stop();
    std::cout << "Center net server stoped..." << std::endl;

    return 0;
}

