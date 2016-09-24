#ifndef _HEADER_H
#define _HEADER_H

#include <string>

namespace easyrpc
{

const int max_buffer_len = 8192;
const int request_header_len = 12;
const int response_header_len = 4;

enum class call_mode : unsigned int
{
    raw,
    non_raw
};

struct request_header
{
    unsigned int protocol_len;
    unsigned int body_len;
    call_mode mode;
};

struct response_header
{
    unsigned int body_len;
};

using one_way = void;
using two_way = std::string;

}

#endif
