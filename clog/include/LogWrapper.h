/* Copyright(C)
* For free
* All right reserved
* 
*/
/**
* @file LogWrapper.h
* @brief 日志库包装接口定义
* @author highway-9, 787280310@qq.com
* @version 1.1.0
* @date 2015-12-05
*/

#ifndef _LOGWRAPPER_H
#define _LOGWRAPPER_H

/// 日志优先级，值越低优先级越高
enum LogPriorityLevel
{
    FatalLevel  = 0,
    AlertLevel  = 100,
    CritLevel   = 200,
    ErrorLevel  = 300,
    WarnLevel   = 400,
    NoticeLevel = 500,
    InfoLevel   = 600,
    DebugLevel  = 700
};

/**
* @brief logPrint 日志打印函数接口
*
* @param filePath 文件路径
* @param function 函数名
* @param line 代码行数
* @param priorityLevel 优先级
* @param format 可变参数的format
* @param ... 可变参数
*/
void logPrint(const char* filePath,
              const char* function,
              unsigned long line,
              unsigned int priorityLevel,
              const char* format, ...);

/// 代码定位宏
#define LOCATION_INFO          __FILE__, __FUNCTION__, __LINE__

#define LOG_FATAL(format, ...)   logPrint(LOCATION_INFO, FatalLevel, format, ##__VA_ARGS__)
#define LOG_ALERT(format, ...)   logPrint(LOCATION_INFO, AlertLevel, format, ##__VA_ARGS__)
#define LOG_CRIT(format, ...)    logPrint(LOCATION_INFO, CritLevel,  format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...)   logPrint(LOCATION_INFO, ErrorLevel, format, ##__VA_ARGS__)
#define LOG_WARN(format, ...)    logPrint(LOCATION_INFO, WarnLevel,  format, ##__VA_ARGS__)
#define LOG_NOTICE(format, ...)  logPrint(LOCATION_INFO, NoticeLevel,format, ##__VA_ARGS__)
#define LOG_INFO(format, ...)    logPrint(LOCATION_INFO, InfoLevel,  format, ##__VA_ARGS__)
#define LOG_DEBUG(format, ...)   logPrint(LOCATION_INFO, DebugLevel, format, ##__VA_ARGS__)

#endif
