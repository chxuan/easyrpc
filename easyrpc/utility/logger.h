/**
 * @file logger.h
 * @brief 日志库
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-05
 */
#pragma once

#include <stdio.h>
#include <string>
#include <sstream>
#include <memory>

enum class log_level
{
    debug = 0,
    info,
    warn,
    error
};

class logger
{
public:
    logger(const std::string& file_path, unsigned long line, log_level level);
    ~logger();

    template<typename T>
    logger& operator << (const T& t)
    {
        (*buffer_) << t;
        return *this;
    }

private:
    std::string get_level_string();
    std::string make_log();
    void print_log(const std::string& log);

private:
    std::shared_ptr<std::ostringstream> buffer_;
    log_level level_;      
};

#define FILE_LOCATION __FILE__, __LINE__

#define log_error logger(FILE_LOCATION, log_level::error)
#define log_warn logger(FILE_LOCATION, log_level::warn)
#define log_info logger(FILE_LOCATION, log_level::info)
#define log_debug logger(FILE_LOCATION, log_level::debug)
