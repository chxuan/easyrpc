#pragma once

#include "crpc/core/protocol/error_code.h"

const int request_header_len = 4 + 4 + 4;
const int response_header_len = 4 + 4;
const int push_header_len = 4 + 4 + 4;
const std::string subscribe_topic_flag = "1";
const std::string cancel_subscribe_topic_flag = "0";

enum class serialize_mode : unsigned int
{
    serialize,
    non_serialize
};

enum class client_type : unsigned int
{
    rpc_client,
    async_rpc_client,
    pub_client,
    sub_client
};

struct client_flag
{
    serialize_mode mode;
    client_type type;
};

struct request_header
{
    unsigned int protocol_len;
    unsigned int message_name_len;
    unsigned int body_len;
};

struct request_content
{
    unsigned int call_id;
    client_flag flag;
    std::string protocol;
    std::string message_name;
    std::string body;
};

struct request_data
{
    request_header header;
    request_content content;
};

struct response_header
{
    unsigned int message_name_len;
    unsigned int body_len;
};

struct response_content
{
    unsigned int call_id;
    rpc_error_code code;
    std::string message_name;
    std::string body;
};

struct response_data
{
    response_header header;
    response_content content;
};

struct push_header
{
    unsigned int protocol_len;
    unsigned int message_name_len;
    unsigned int body_len;
};

struct push_content
{
    serialize_mode mode;
    std::string protocol;
    std::string message_name;
    std::string body;
};

struct push_data
{
    push_header header;
    push_content content;
};

