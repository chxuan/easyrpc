/*************************************************************************
	> File Name: main.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年01月10日 星期日 14时00分15秒
 ************************************************************************/

#include <iostream>
#include "ProtolDef.h"
#include "RCFClientWrapper.hpp"

int main()
{
    RCFClientWrapper<I_RCFMessageHandler> client("127.0.0.1", 50001);

    try
    {
        client.rcfClientObject()->print("Hello world");
    }
    catch (const RCF::Exception& e)
    {
        std::cout << "Error: " << e.getErrorString() << std::endl;
    }

    return 0;
}

