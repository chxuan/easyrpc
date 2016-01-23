/*************************************************************************
	> File Name: main.cpp
	> Author: 
	> Mail: 
	> Created Time: 2015年12月07日 星期一 21时10分35秒
 ************************************************************************/

#include <iostream>
#include <string>
#include "LogWrapper.h"

int main(int argc, char* argv[])
{
    // C语言风格
    LOG_FATAL("Fatal log");
    LOG_ALERT("Alert log");
    LOG_CRIT("Crit log");
    LOG_ERROR("Error log");
    LOG_WARN("Warn log");
    LOG_NOTICE("Notice log");

    int sum = 10;
    LOG_INFO("a + b = %d", sum);

    std::string str = "Debug log";
    LOG_DEBUG("message: %s", str.c_str());

    // C++风格
    LOG_FATAL_STREAM() << "Hello world";
    LOG_ALERT_STREAM() << "Hello world";
    LOG_CRIT_STREAM() << "Hello world";
    LOG_ERROR_STREAM() << "Hello world";
    LOG_NOTICE_STREAM() << "Hello world";
    LOG_INFO_STREAM() << "a + b = " << sum;
    LOG_DEBUG_STREAM() << "message: " << str;

    return 0;
}
