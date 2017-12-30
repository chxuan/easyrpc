#include "logger.h"
#include "utiltiy.h"

logger::logger(const std::string& file_path, unsigned long line, log_level level) 
    : level_(level)
{
    buffer_ = std::make_shared<std::ostringstream>();
    int pos = file_path.find_last_of("/");
    (*buffer_) << file_path.substr(pos + 1) << ":" << line << " ";
}

logger::~logger()
{
    print_log(make_log());
}

std::string logger::get_level_string()
{
    switch (level_)
    {
    case log_level::error: return "[error]";
    case log_level::warn: return "[warn]";
    case log_level::info: return "[info]";
    case log_level::debug: return "[debug]";
    default: return "[debug]";
    }
}

std::string logger::make_log()
{
    std::string log;
    log += utiltiy::get_time_us();
    log += " ";
    log += get_level_string();
    log += " ";
    log += buffer_->str();

    return log;
}

void logger::print_log(const std::string& log)
{
    switch (level_)
    {
    case log_level::error: printf("\033[31m%s\n\033[0m", log.c_str()); break;
    case log_level::warn: printf("\033[33m%s\n\033[0m", log.c_str()); break;
    default: printf("%s\n", log.c_str()); break;
    }
}
