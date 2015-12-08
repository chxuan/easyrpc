/* Copyright(C)
* For free
* All right reserved
* 
*/
/**
* @file LogImpl.h
* @brief 日志库接口定义
* @author highway-9, 787280310@qq.com
* @version 0.1.0
* @date 2015-12-05
*/

#ifndef _LOGIMPL_H
#define _LOGIMPL_H

#include <log4cpp/Category.hh>
#include <log4cpp/Appender.hh>
#include <log4cpp/Priority.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/RollingFileAppender.hh>
#include <log4cpp/OstreamAppender.hh>
#include <string>

class LogImpl
{
  public:
    static LogImpl* getInstance();

    bool printToFile(unsigned int priorityLevel, const std::string& logContent);
    bool printToConsole(unsigned int priorityLevel, const std::string& logContent);

  private:
    bool init();
    void deinit();
    std::string createLogFile();
    bool initLogCore(const std::string& logFileName);
    bool initLogFile(const std::string& logFileName);
    bool initLogConsole();

  private:
    LogImpl();
    ~LogImpl();
    LogImpl(const LogImpl& var);
    LogImpl& operator = (const LogImpl& var);

  private:
    log4cpp::Category*      m_fileRoot;
    log4cpp::Category*      m_consoleRoot;
    bool                    m_isInitSuccess;
};

#endif
