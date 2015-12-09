/*************************************************************************
	> File Name: main.cpp
	> Author: 
	> Mail: 
	> Created Time: 2015年12月07日 星期一 21时10分35秒
 ************************************************************************/

#include <iostream>
using namespace std;
#include "LogWrapper.h"

class TestA
{
  public:
    TestA()
    {
        LOG_INFO("Create TestA");
    }

    void printLog()
    {
        LOG_DEBUG("Hello log4cpp");
    }

    ~TestA()
    {
        LOG_INFO("Delete TestA");
    }
};

int main()
{
    TestA a;
    a.printLog();
    LOG_FATAL("Hello world");
    LOG_ALERT("Hello world");
    LOG_CRIT("Hello world");
    LOG_ERROR("Hello world");
    LOG_WARN("Hello world");
    LOG_NOTICE("Hello world");
    LOG_INFO("Hello world");
    LOG_DEBUG("Hello world");
    return 0;
}
