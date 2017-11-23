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

