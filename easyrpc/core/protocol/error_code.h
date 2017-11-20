/**
 * @file error_code.h
 * @brief crpc内部错误码定义
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-05
 */
#pragma once

#include <string>
#include <map>

enum class rpc_error_code : int 
{
    ok = 0,
    route_failed = 1,
    request_timeout = 2
};

static std::map<rpc_error_code, std::string> rpc_error_map
{
    {rpc_error_code::ok, "OK"},
    {rpc_error_code::route_failed, "Route failed"},
    {rpc_error_code::request_timeout, "Request timeout"}
};

inline std::string get_rpc_error_string(rpc_error_code error_code)
{
    auto iter = rpc_error_map.find(error_code);
    if (iter != rpc_error_map.end())
    {
        return iter->second;
    }

    return "";
}

class error_code
{
public:
    error_code() = default;
    error_code(rpc_error_code code) : code_(code) {}

    int code() const { return static_cast<int>(code_); }
    std::string message() const { return get_rpc_error_string(code_); }
    explicit operator bool() const { return code_ != rpc_error_code::ok ? true : false; }

private:
    rpc_error_code code_ = rpc_error_code::ok;
};
