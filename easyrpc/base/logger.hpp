#ifndef _LOGGER_H
#define _LOGGER_H

#include <iostream>
#include <spdlog/spdlog.h>
#include "file_util.hpp"

namespace easyrpc
{

static const std::string logger_name = "easyrpc";
static const std::size_t max_file_size = 3 * 1024 * 1024;
static const std::size_t max_files = 30;

class logger_impl
{
public:
    logger_impl(const logger_impl&) = delete;
    logger_impl& operator=(const logger_impl&) = delete;
    logger_impl()
    {
        init();
    }

    static logger_impl& instance()
    {
        static logger_impl logger;
        return logger;
    }

    std::shared_ptr<spdlog::logger> get_console_logger()
    {
        return _console_logger;
    }

    std::shared_ptr<spdlog::logger> get_file_logger()
    {
        return _file_logger;
    }

private:
    bool init()
    {
        std::string file_name = create_log_file();
        if (file_name.empty())
        {
            return false;
        }
        return init_log_core(file_name);
    }

    std::string create_log_file()
    {
        std::string exe_path = file_util::current_exe_path();
        if (exe_path.empty())
        {
            return "";
        }

        std::string log_path;
        if (exe_path[exe_path.size() - 1] == '/' || exe_path[exe_path.size() - 1] == '\\')
        {
            log_path = exe_path + "logs";
        }
        else
        {
            log_path = exe_path + "/logs";
        }

        if (!file_util::mkdir(log_path))
        {
            return "";
        }       

        std::string exe_name = file_util::current_exe_name();
        if (exe_name.empty())
        {
            return "";
        }
        return log_path + "/" + exe_name + "_" + logger_name;
    }

    bool init_log_core(const std::string& file_name)
    {
        try
        {
            _console_logger = spdlog::stdout_logger_mt("console", false);
            _file_logger = spdlog::rotating_logger_mt("file", file_name, max_file_size, max_files, true);
            _console_logger->set_level(spdlog::level::debug); 
            _file_logger->set_level(spdlog::level::debug); 
        }
        catch (std::exception&)
        {
            return false;
        }

        return true;       
    }

private:
    std::shared_ptr<spdlog::logger> _console_logger;
    std::shared_ptr<spdlog::logger> _file_logger;
};

class logger
{
public:
    logger() = default;
    logger(const logger&) = delete;
    logger& operator=(const logger&) = delete;
    logger(const char* file_path, const char* func_name, unsigned long line, spdlog::level::level_enum level)
        : _file_path(file_path), _file_name(func_name), _line(line), _level(level) {}

    template<typename... Args>
    void log(const std::string& fmt, Args&&... args)
    {
        log(fmt.c_str(), std::forward<Args>(args)...);
    }

    template<typename... Args>
    void log(const char* fmt, Args&&... args)
    {
        const std::string& content = make_content(fmt);
        logger_impl::instance().get_console_logger()->log(_level, content.c_str(), std::forward<Args>(args)...);
        logger_impl::instance().get_file_logger()->log(_level, content.c_str(), std::forward<Args>(args)...);
    }

private:
    std::string make_content(const std::string& fmt)
    {
#ifdef _WIN32
        int pos = _file_path.find_last_of("\\");
#else
        int pos = _file_path.find_last_of("/");
#endif
        std::string content = _file_path.substr(pos + 1) + " " + _file_name + "(" + std::to_string(_line) + ") " + fmt;
        return content;
    }

private:
    std::string _file_path;
    std::string _file_name;
    unsigned long _line;
    spdlog::level::level_enum _level;
};

#define LOCATION       __FILE__, __FUNCTION__, __LINE__
#define log_trace       logger(LOCATION, spdlog::level::level_enum::trace).log
#define log_debug       logger(LOCATION, spdlog::level::level_enum::debug).log
#define log_info        logger(LOCATION, spdlog::level::level_enum::info).log
#define log_warn        logger(LOCATION, spdlog::level::level_enum::warn).log
#define log_error       logger(LOCATION, spdlog::level::level_enum::err).log
#define log_critical    logger(LOCATION, spdlog::level::level_enum::critical).log

}

#endif
