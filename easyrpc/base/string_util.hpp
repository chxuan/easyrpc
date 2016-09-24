#ifndef _STRIN_GUTIL_H
#define _STRIN_GUTIL_H

#include <string.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

namespace easyrpc
{

class string_util
{
public:
    static std::string trim_left(const std::string& str, const std::string& token = " ")
    {
        std::string t = str;
        t.erase(0, t.find_first_not_of(token));
        return t;
    }

    static std::string trim_right(const std::string& str, const std::string& token = " ")
    {
        std::string t = str;
        t.erase(t.find_last_not_of(token) + 1);
        return t;
    }

    static std::string trim(const std::string& str, const std::string& token = " ")
    {
        std::string t = str;
        t.erase(0, t.find_first_not_of(token));
        t.erase(t.find_last_not_of(token) + 1);
        return t;
    }

    static std::string tolower(const std::string& str)
    {
        std::string t = str;
        std::transform(t.begin(), t.end(), t.begin(), ::tolower);
        return t;
    }

    static std::string toupper(const std::string& str)
    {
        std::string t = str;
        std::transform(t.begin(), t.end(), t.begin(), ::toupper);
        return t;
    }

    static bool	starts_with(const std::string& str, const std::string& substr)
    {
        return str.find(substr) == 0;
    }

    static bool ends_with(const std::string& str, const std::string& substr)
    {
        return str.rfind(substr) == (str.length() - substr.length());
    }

    static bool equals_ignore_case(const std::string& str1, const std::string& str2) 
    {
        return tolower(str1) == tolower(str2);
    }

    static std::vector<std::string> split(const std::string& str, const std::string& delimiter)
    {
        char* save = nullptr;
#ifdef _WIN32
        char* token = strtok_s(const_cast<char*>(str.c_str()), delimiter.c_str(), &save);
#else
        char* token = strtok_r(const_cast<char*>(str.c_str()), delimiter.c_str(), &save);
#endif
        std::vector<std::string> result;
        while (token != nullptr)
        {
            result.emplace_back(token);
#ifdef _WIN32
            token = strtok_s(nullptr, delimiter.c_str(), &save);
#else
            token = strtok_r(nullptr, delimiter.c_str(), &save);
#endif
        }
        return result;
    }

    static bool contains(const std::string& str, const std::string& token)
    {
        return str.find(token) == std::string::npos ? false : true;
    }
};

}

#endif
