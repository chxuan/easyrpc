/*************************************************************************
	> File Name: main.cpp
	> Author: 
	> Mail: 
	> Created Time: 2015年12月07日 星期一 21时10分35秒
 ************************************************************************/

#include <iostream>
using namespace std;
#include "LogWrapper.h"

int main()
{
    int a = 1;
    LOG_DEBUG("Hello world");
    LOG_INFO("nihao a: %d", a);
    LOG_WARN("hello world");
    LOG_ERROR("nihao a: %d", a);
    return 0;
}
