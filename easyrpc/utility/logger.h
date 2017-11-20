/**
 * @file logger.h
 * @brief 日志库
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-05
 */
#pragma once

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif
#include <stdio.h>
#include <ctime>
#include <string>
#include <sstream>
#include <memory>

enum class priority_level
{
    error = 0,
    warn = 1,
    info = 2,
    debug = 3
};

#ifdef _WIN32
inline void gettimeofday(struct timeval* tv)
{
    uint64_t intervals;
    FILETIME ft;

    GetSystemTimeAsFileTime(&ft);
    intervals = (static_cast<uint64_t>(ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
    intervals -= 116444736000000000;
    tv->tv_sec = static_cast<long>(intervals / 10000000);
    tv->tv_usec = static_cast<long>((intervals % 10000000) / 10);
}
#endif

class logger_stream
{
public:
    logger_stream(const std::string& file_path, 
                  const std::string& func_name, 
                  unsigned long line, 
                  priority_level level);
    ~logger_stream();

    template<typename T>
    logger_stream& operator << (const T& t)
    {
        (*buffer_) << t;
        return *this;
    }

private:
    std::string get_current_time_us();
    std::string level_enum_to_string();
    std::string make_log();
    void print_log(const std::string& log);

private:
    std::shared_ptr<std::ostringstream> buffer_;
    priority_level level_;      
};

class logger
{
public:
    logger(const char* file_path, 
           const char* func_name, 
           unsigned long line, 
           priority_level level);
    logger_stream log() const;

private:
    std::string file_path_;             
    std::string func_name_;             
    unsigned long line_;          
    priority_level level_; 
};

#define LOCATION_INFO __FILE__, __FUNCTION__, __LINE__

#define log_error       logger(LOCATION_INFO, priority_level::error).log
#define log_warn        logger(LOCATION_INFO, priority_level::warn).log
#define log_info        logger(LOCATION_INFO, priority_level::info).log
#define log_debug       logger(LOCATION_INFO, priority_level::debug).log
