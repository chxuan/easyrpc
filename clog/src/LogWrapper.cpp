/* Copyright(C)
 * For free
 * All right reserved
 * 
 */
/**
 * @file LogWrapper.cpp
 * @brief 日志库包装接口实现
 * @author highway-9, 787280310@qq.com
 * @version 0.1.0
 * @date 2015-12-05
 */

#include "LogWrapper.h"
#include "LogImpl.h"
#include "Utils.h"
#include <stdio.h>
#include <stdarg.h>
#include <iostream>
#include <string>

static std::string createLogContent(const char* filePath,
                                    const char* function,
                                    unsigned long line,
                                    unsigned int priorityLevel,
                                    const char* content)
{
    std::string strLine = utils::String::uint64ToString(line);
    std::string logContent = std::string(filePath) + " " + function + "(" + strLine + ") " + content;
    return logContent;
}

static void logPrintImpl(const char* filePath,
                    const char* function,
                    unsigned long line,
                    unsigned int priorityLevel,
                    const char* content)
{
    std::string logContent = createLogContent(filePath, function, line, priorityLevel, content);
/*
    bool ok = LogImpl::getInstance()->printToFile(priorityLevel, logContent);
    if (!ok)
    {
        std::cout << "Print log to file failed, log: " << logContent << std::endl;
    }
*/
    bool ok = LogImpl::getInstance()->printToConsole(priorityLevel, logContent);
    if (!ok)
    {
        std::cout << "Print log to console failed, log: " << logContent << std::endl;
    }
}

void logPrint(const char* filePath,
              const char* function,
              unsigned long line,
              unsigned int priorityLevel,
              const char* format, ...)
{
    char buf[4096] = {'\0'};
    va_list args;
    va_start(args, format);
    vsprintf(buf, format, args);
    va_end(args);

    logPrintImpl(filePath, function, line, priorityLevel, buf);
}
