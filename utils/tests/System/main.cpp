/*************************************************************************
	> File Name: main.cpp
	> Author: 
	> Mail: 
	> Created Time: 2015年11月29日 星期日 18时41分08秒
 ************************************************************************/

#include <iostream>
using namespace std;

#include "Utils.h"

int main()
{
    std::cout << "####################uuid###################" << std::endl;
    for (int i = 0; i < 10; ++i)
    {
        std::cout << utils::System::uuid() << std::endl;
    }

    std::cout << "########################md5##################" << std::endl;

    std::string str = "hello";
    std::cout << str << ": " << utils::System::md5(str) << std::endl;
    str = "中国";
    std::cout << str << ": " << utils::System::md5(str) << std::endl;

    std::cout << "####################sha1#####################" << std::endl;

    str = "hello";
    std::cout << str << ": " << utils::System::sha1(str) << std::endl;
    str = "中国";
    std::cout << str << ": " << utils::System::sha1(str) << std::endl;

    return 0;
}
