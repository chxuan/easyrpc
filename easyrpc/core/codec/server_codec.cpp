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
    prepare_decode_header();
}

void server_codec::decode_header(const std::vector<char>& buffer)
{
    memcpy(&header_, &buffer[0], sizeof(header_));
    prepare_decode_body();
}

void server_codec::decode_body(const std::vector<char>& buffer)
{
    int pos = 0;

    copy_from_buffer(body_.serial_num, pos, buffer);
    copy_from_buffer(body_.func_id, pos, buffer);
    copy_from_buffer(body_.message_name, pos, header_.message_name_len, buffer);
    copy_from_buffer(body_.message_data, pos, header_.message_data_len, buffer);

    prepare_decode_header();
    decode_data_callback_(body_);
}

void server_codec::prepare_decode_header()
{
    decode_header_ = true;
    next_recv_bytes_ = request_header_len;
}

void server_codec::prepare_decode_body()
{
    decode_header_ = false;
    next_recv_bytes_ = 2 * sizeof(int) + header_.message_name_len + header_.message_data_len;
}
