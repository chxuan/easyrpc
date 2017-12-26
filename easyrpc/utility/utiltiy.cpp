#include "utiltiy.h"

std::vector<std::string> utiltiy::split(const std::string& str, const std::string& delimiter)
{
    std::string save_str = str;
    char* save = nullptr;

    char* token = strtok_r(const_cast<char*>(save_str.c_str()), delimiter.c_str(), &save);
    std::vector<std::string> results;
    while (token != nullptr)
    {
        results.emplace_back(token);
        token = strtok_r(nullptr, delimiter.c_str(), &save);
    }

    return results;
}

bool utiltiy::contains(const std::string& str, const std::string& token)
{
    return str.find(token) == std::string::npos ? false : true;
}

bool utiltiy::get_ip_and_port(const std::string& address, std::string& ip, unsigned short& port)
{
    if (!contains(address, ":"))
    {
        return false;
    }

    auto vec = split(address, ":");
    if (vec.size() != 2)
    {
        return false;
    }

    ip = vec[0];
    port = std::atoi(vec[1].c_str());

    return true;
}

std::string utiltiy::get_time_us()
{
    struct timeval now_tv;
    gettimeofday(&now_tv, nullptr);

    struct tm t;
    localtime_r(&now_tv.tv_sec, &t);

    char time_str[27] = {0};
    snprintf(time_str, sizeof(time_str), "%04d-%02d-%02d %02d:%02d:%02d.%06d",
             t.tm_year + 1900,
             t.tm_mon + 1,
             t.tm_mday,
             t.tm_hour,
             t.tm_min,
             t.tm_sec,
             static_cast<int>(now_tv.tv_usec));

    return time_str;
}
