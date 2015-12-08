/* Copyright(C)
* For free
* All right reserved
* 
*/
/**
* @file LogImpl.cpp
* @brief 日志库接口实现
* @author highway-9, 787280310@qq.com
* @version 0.1.0
* @date 2015-12-05
*/

#include "LogImpl.h"
#include "Utils.h"
#include <stdio.h>
#include <iostream>

LogImpl::LogImpl()
    : m_fileRoot(NULL),
    m_consoleRoot(NULL),
    m_isInitSuccess(false)
{
    m_isInitSuccess = init();
    if (!m_isInitSuccess)
    {
        std::cout << "Init log failed" << std::endl;
    }
}

LogImpl::~LogImpl()
{
    deinit();
}

LogImpl* LogImpl::getInstance()
{
    static LogImpl logImpl;
    return &logImpl;
}

bool LogImpl::printToFile(unsigned int priorityLevel, const std::string& logContent)
{
    if (!m_isInitSuccess)
    {
        return false;
    }

    m_fileRoot->log(priorityLevel, logContent);

    return true;
}

bool LogImpl::printToConsole(unsigned int priorityLevel, const std::string& logContent)
{
    if (!m_isInitSuccess)
    {
        return false;
    }

    m_consoleRoot->log(priorityLevel, logContent);

    return true;
}

bool LogImpl::init()
{
    std::string logFileName = createLogFile();
    if (logFileName.empty())
    {
        std::cout << "Create log file failed" << std::endl;
        return false;
    }

    bool ok = initLogCore(logFileName);
    if (!ok)
    {
        std::cout << "Init log core failed" << std::endl;
        return false;
    }

    return true;
}

void LogImpl::deinit()
{
    log4cpp::Category::shutdown();
}

std::string LogImpl::createLogFile()
{
    // 获取当前可执行文件所在路径
    std::string exePath = utils::FileSystem::currentExePath();
    if (exePath.empty())
    {
        std::cout << "Get current exe path failed" << std::endl;
        return std::string();
    }

    // 创建日志路径
    std::string logPath = exePath + "/logs";
    bool ok = utils::FileSystem::mkdir(logPath);
    if (!ok)
    {
        std::cout << "Create log path failed, log path: " << logPath << std::endl;
        return std::string();
    }

    // 获取当前可执行文件名
    std::string exeName = utils::FileSystem::currentExeName();
    if (exeName.empty())
    {
        std::cout << "Get current exe name failed" << std::endl;
        return std::string();
    }

    // 日志文件名
    std::string logFileName = logPath + "/" + exeName + ".log";

    return logFileName;
    
}

bool LogImpl::initLogCore(const std::string& logFileName)
{
    bool ok = initLogFile(logFileName);
    if (!ok)
    {
        std::cout << "Init log file failed, log file name: " << logFileName << std::endl;
        return false;
    }

    ok = initLogConsole();
    if (!ok)
    {
        std::cout << "Init log console failed" << std::endl;
        return false;
    }

    return true;
}

bool LogImpl::initLogFile(const std::string& logFileName)
{
    log4cpp::PatternLayout* patternLayout = NULL;
    patternLayout = new log4cpp::PatternLayout();
    if (patternLayout == NULL)
    {
        return false;
    }

    patternLayout->setConversionPattern("%d: [%-5p] %c%x: %m%n");

    log4cpp::RollingFileAppender* rollfileAppender = NULL;
    rollfileAppender = new log4cpp::RollingFileAppender("rollfileAppender", logFileName, 5*1024*1024, 1);
    if (rollfileAppender == NULL)
    {
        return false;
    }

    rollfileAppender->setMaxBackupIndex(30);
    rollfileAppender->setLayout(patternLayout);

    m_fileRoot = &(log4cpp::Category::getRoot().getInstance(""));
    if (m_fileRoot == NULL)
    {
        return false;
    }

    m_fileRoot->addAppender(rollfileAppender);
    m_fileRoot->setPriority(log4cpp::Priority::DEBUG);

    return true;
}

bool LogImpl::initLogConsole()
{
    log4cpp::PatternLayout* patternLayout = NULL;
    patternLayout = new log4cpp::PatternLayout();
    if (patternLayout == NULL)
    {
        return false;
    }

    patternLayout->setConversionPattern("%d: [%-5p] %c%x: %m%n");

    log4cpp::OstreamAppender* osAppender = NULL;
    osAppender = new log4cpp::OstreamAppender("osAppender", &std::cout);
    if (osAppender == NULL)
    {
        return false;
    }

    osAppender->setLayout(patternLayout);

    m_consoleRoot = &(log4cpp::Category::getRoot().getInstance(""));
    if (m_consoleRoot == NULL)
    {
        return false;
    }

    m_consoleRoot->addAppender(osAppender);
    m_consoleRoot->setPriority(log4cpp::Priority::DEBUG);
    
    return true;
}
