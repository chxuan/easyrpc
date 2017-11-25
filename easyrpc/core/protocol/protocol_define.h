/**
 * @file protocol_define.h
 * @brief easyrpc内部消息格式
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-25
 */
#pragma once

#include "easyrpc/core/protocol/error_code.h"

const int request_header_len = 4 + 4;
const int response_header_len = 4 + 4;

struct request_header
{
    int message_name_len;
    int message_data_len;
};

struct request_body
{
    int serial_num;
    int func_id;
    std::string message_name;
    std::string message_data;
};

using response_header = request_header;

struct response_body
{
    int serial_num;
    rpc_error_code code;
    std::string message_name;
    std::string message_data;
};

