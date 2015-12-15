/* Copyright(C)
 * For free
 * All right reserved
 * 
 */
/**
 * @file LogWrapper.cpp
 * @brief 日志库包装接口实现
 * @author highway-9, 787280310@qq.com
 * @version 1.1.0
 * @date 2015-12-05
 */

#include "LogWrapper.h"
#include "LogImpl.h"
#include "Utils.h"
#include <stdio.h>
#include <stdarg.h>
#include <iostream>
#include <string>

/**
* @brief createLogContent 创建一个日志文本
*
* @param filePath 文件路径
* @param function 函数名
* @param line 代码行数
* @param priorityLevel 日志优先级
* @param content 日志输入文本
*
* @return 一个带有日志定为功能的日志文本
*/
static std::string createLogContent(const char* filePath,
                                    const char* function,
                                    unsigned long line,
                                    unsigned int priorityLevel,
                                    const char* content)
{
    std::string strLine = utils::String::uint64ToString(line);

    std::string strFilePath(filePath);
    int pos = strFilePath.find_last_of("/");
    std::string logContent = strFilePath.substr(pos + 1) + " " + function + "(" + strLine + ") " + content;

    return logContent;
}

/**
* @brief logPrintImpl 日志打印函数
*
* @param filePath 文件路径
* @param function 函数名
* @param line 代码行数
* @param priorityLevel 日志优先级
* @param content 日志输入文本
*/
static void logPrintImpl(const char* filePath,
                    const char* function,
                    unsigned long line,
                    unsigned int priorityLevel,
                    const char* content)
{
    std::string logContent = createLogContent(filePath, function, line, priorityLevel, content);

    bool ok = LogImpl::getInstance()->logPrint(priorityLevel, logContent);
    if (!ok)
    {
        std::cout << "Print log to file failed, log: " << logContent << std::endl;
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
