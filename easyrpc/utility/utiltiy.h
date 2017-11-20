/**
 * @file utiltiy.h
 * @brief 通用工具
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-05
 */
#pragma once

#include <string.h>
#include <string>
#include <vector>

inline std::vector<std::string> split(const std::string& str, const std::string& delimiter)
{
    std::string save_str = str;
    char* save = nullptr;

#ifdef _WIN32
    char* token = strtok_s(const_cast<char*>(save_str.c_str()), delimiter.c_str(), &save);
#else
    char* token = strtok_r(const_cast<char*>(save_str.c_str()), delimiter.c_str(), &save);
#endif

    std::vector<std::string> results;
    while (token != nullptr)
    {
        results.emplace_back(token);
#ifdef _WIN32
        token = strtok_s(nullptr, delimiter.c_str(), &save);
#else
        token = strtok_r(nullptr, delimiter.c_str(), &save);
#endif
    }

    return results;
}

inline bool contains(const std::string& str, const std::string& token)
{
    return str.find(token) == std::string::npos ? false : true;
}

inline bool get_ip_and_port(const std::string& address, std::string& ip, unsigned short& port)
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

