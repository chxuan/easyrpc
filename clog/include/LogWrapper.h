/* Copyright(C)
* For free
* All right reserved
* 
*/
/**
* @file LogWrapper.h
* @brief 日志库包装接口定义
* @author highway-9, 787280310@qq.com
* @version 0.1.0
* @date 2015-12-05
*/

#ifndef _LOGWRAPPER_H
#define _LOGWRAPPER_H

/// 日志优先级，值越低优先级越高
enum LogPriorityLevel
{
    EmergLevel  = 0,
    FatalLevel  = 10,
    AlertLevel  = 100,
    CritLevel   = 200,
    ErrorLevel  = 300,
    WarnLevel   = 400,
    NoticeLevel = 500,
    InfoLevel   = 600,
    DebugLevel  = 700,
    NotsetLevel = 800
};

void logPrint(const char* filePath,
              const char* function,
              unsigned long line,
              unsigned int priorityLevel,
              const char* format, ...);

#define LOCATION_INFO          __FILE__, __FUNCTION__, __LINE__

#define LOG_ERROR(format, ...) logPrint(LOCATION_INFO, ErrorLevel, format, ##__VA_ARGS__)
#define LOG_WARN(format, ...)  logPrint(LOCATION_INFO, WarnLevel,  format, ##__VA_ARGS__)
#define LOG_INFO(format, ...)  logPrint(LOCATION_INFO, InfoLevel,  format, ##__VA_ARGS__)
#define LOG_DEBUG(format, ...) logPrint(LOCATION_INFO, DebugLevel, format, ##__VA_ARGS__)

#endif
