#include "codec.h"
#include "easyrpc/utility/logger.h"

codec::codec()
{
    reset();
}

std::shared_ptr<std::string> codec::encode(int serial_num, 
                                           int func_id,
                                           const std::shared_ptr<google::protobuf::Message>& message) 
{
    auto body = encode_body(serial_num, func_id, message);
    auto header = encode_header(body);
    return make_network_data(header, body);
}

void codec::decode(const std::vector<char>& buffer, const std::shared_ptr<tcp_session>& session)
{
    if (decode_header_)
    {
        decode_header(buffer);
    }
    else
    {
        decode_body(buffer, session);
    }
}

void codec::reset()
{
    prepare_decode_header();
}

int codec::get_next_recv_bytes()
{
    return next_recv_bytes_;
}

packet_header codec::encode_header(const packet_body& body)
{
    packet_header header;
    header.message_name_len = body.message_name.size();
    header.message_data_len = body.message_data.size();

    return header;
}

packet_body codec::encode_body(int serial_num, 
                               int func_id, 
                               const std::shared_ptr<google::protobuf::Message>& message)
{
    packet_body body;
    body.serial_num = serial_num;
    body.func_id = func_id;
    body.message_name = message->GetDescriptor()->full_name();
    body.message_data = protobuf_serialize::serialize(message);

    return body;
}

std::shared_ptr<std::string> codec::make_network_data(const packet_header& header, const packet_body& body)
{
    auto network_data = std::make_shared<std::string>();

    copy_to_buffer(header, network_data);
    copy_to_buffer(body.serial_num, network_data);
    copy_to_buffer(body.func_id, network_data);
    copy_to_buffer(body.message_name, network_data);
    copy_to_buffer(body.message_data, network_data);

    return network_data;
}

void codec::decode_header(const std::vector<char>& buffer)
{
    copy_from_buffer(header_, buffer);
    if (is_vaild_header(header_.message_name_len, header_.message_data_len))
    {
        prepare_decode_body();
    }
    else
    {
        reset();
        log_error() << "invaild header, message_name_len: " << header_.message_name_len << 
            ", message_data_len: " << header_.message_data_len;
    }
}

void codec::decode_body(const std::vector<char>& buffer, const std::shared_ptr<tcp_session>& session)
{
    int pos = 0;

    copy_from_buffer(body_.serial_num, pos, buffer);
    copy_from_buffer(body_.func_id, pos, buffer);
    copy_from_buffer(body_.message_name, pos, header_.message_name_len, buffer);
    copy_from_buffer(body_.message_data, pos, header_.message_data_len, buffer);

    prepare_decode_header();

    handle_decode_data(body_, session);
}

void codec::prepare_decode_header()
{
    decode_header_ = true;
    next_recv_bytes_ = sizeof(packet_header);
}

void codec::prepare_decode_body()
{
    decode_header_ = false;
    next_recv_bytes_ = 2 * sizeof(int) + header_.message_name_len + header_.message_data_len;
}

void codec::copy_from_buffer(std::string& str, int& pos, int len, const std::vector<char>& buffer)
{
    str.assign(&buffer[pos], len);
    pos += len;
}

void codec::copy_to_buffer(const std::string& str, std::shared_ptr<std::string>& buffer)
{
    buffer->append(str);
}

bool codec::is_vaild_header(int message_name_len, int message_data_len)
{
    static const int max_message_name_len = 256;
    static const int max_message_data_len = 20 * 1024 * 1024;

    if (message_name_len >= 0 && message_name_len < max_message_name_len && 
        message_data_len >= 0 && message_data_len < max_message_data_len)
    {
        return true;
    }

    return false;
}
