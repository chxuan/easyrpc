/*************************************************************************
	> File Name: main.cpp
	> Author: 
	> Mail: 
	> Created Time: 2015年12月07日 星期一 21时10分35秒
 ************************************************************************/

#include <iostream>
#include <string>
#include "LogWrapper.h"

int main()
{
    // C风格
    logFatal("Fatal log");
    logAlert("Alert log");
    logCrit("Crit log");
    logError("Error log");
    logWarn("Warn log");
    logNotice("Notice log");

    int sum = 10;
    logInfo("a + b = %d", sum);

    std::string str = "Debug log";
    logDebug("message: %s", str.c_str());

    // C++风格
    logFatal() << "Hello world";
    logAlert() << "Hello world";
    logCrit() << "Hello world";
    logError() << "Hello world";
    logWarn() << "Hello world";
    logNotice() << "a + b = " << sum;
    logDebug() << "message: " << str;

    return 0;
}
