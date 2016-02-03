/*************************************************************************
	> File Name: main.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年02月03日 星期三 15时06分27秒
 ************************************************************************/

#include <iostream>
#include "PeopleInfoMessage.h"
#include "TcpServerImpl.h"

int main()
{
    std::cout << "Server starting..." << std::endl;
    TcpServerImpl server(8888);
    server.start();

    std::cout << "###############################" << std::endl;

    //std::cin.get();
    boost::this_thread::sleep_for(boost::chrono::milliseconds(5000));

    std::cout << "Server stoped..." << std::endl;

    return 0;
}
