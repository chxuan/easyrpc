/**
 * @file utiltiy.h
 * @brief 通用工具
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-05
 */
#pragma once

#include <sys/time.h>
#include <string.h>
#include <string>
#include <vector>

class utiltiy
{
public:
    static std::vector<std::string> split(const std::string& str, const std::string& delimiter);
    static bool contains(const std::string& str, const std::string& token);
    static bool get_ip_and_port(const std::string& address, std::string& ip, unsigned short& port);
    static std::string get_time_us();
};

