#include "server_codec.h"

server_codec::server_codec()
{
    reset();
}

server_codec::~server_codec()
{

}

void server_codec::decode(const std::vector<char>& buffer)
{
    if (decode_header_)
    {
        decode_header(buffer);
    }
    else
    {
        decode_body(buffer);
    }
}

void server_codec::reset()
{
    decode_header_ = true;
    next_recv_bytes_ = request_header_len;
}

void server_codec::decode_header(const std::vector<char>& buffer)
{
    memcpy(&header_, &buffer[0], sizeof(header_));
    decode_header_ = false;
    next_recv_bytes_ = 2 * sizeof(std::size_t) + header_.message_name_len + header_.message_data_len;
}

void server_codec::decode_body(const std::vector<char>& buffer)
{
    std::size_t pos = 0;

    memcpy(&body_.serial_num, &buffer[pos], sizeof(body_.serial_num));
    pos += sizeof(body_.serial_num);

    memcpy(&body_.func_id, &buffer[pos], sizeof(body_.func_id));
    pos += sizeof(body_.func_id);

    body_.message_name.assign(&buffer[pos], header_.message_name_len);
    pos += sizeof(header_.message_name_len);

    body_.message_data.assign(&buffer[pos], header_.message_data_len);

    reset();
    decode_data_callback_(body_);
}

