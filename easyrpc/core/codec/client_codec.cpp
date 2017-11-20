#include "client_codec.h"

client_codec::client_codec()
{
    reset();
}

client_codec::~client_codec()
{

}

std::shared_ptr<std::string> client_codec::encode(int serial_num, 
                                                  int func_id,
                                                  const std::shared_ptr<google::protobuf::Message>& message)
{
    auto body = encode_body(serial_num, func_id, message);
    auto header = encode_header(body);
    return make_network_data(header, body);
}

void client_codec::decode(const std::vector<char>& buffer)
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

void client_codec::reset()
{
    decode_header_ = true;
    next_recv_bytes_ = response_header_len;
}

void client_codec::set_decode_data_callback(const std::function<void(const response_body&)>& func)
{
    decode_data_callback_ = func;
}

request_header client_codec::encode_header(const request_body& body)
{
    request_header header;
    header.message_name_len = body.message_name.size();
    header.message_data_len = body.message_data.size();

    return header;
}

request_body client_codec::encode_body(int serial_num, 
                                       int func_id, 
                                       const std::shared_ptr<google::protobuf::Message>& message)
{
    request_body body;
    body.serial_num = serial_num;
    body.func_id = func_id;
    body.message_name = message->GetDescriptor()->full_name();
    body.message_data = protobuf_serialize::serialize(message);

    return body;
}

std::shared_ptr<std::string> client_codec::make_network_data(const request_header& header, const request_body& body)
{
    auto network_data = std::make_shared<std::string>();
    network_data->append(reinterpret_cast<const char*>(&header), sizeof(header));
    network_data->append(reinterpret_cast<const char*>(&body.serial_num), sizeof(body.serial_num));
    network_data->append(body.message_name);
    network_data->append(body.message_data);

    return network_data;
}

void client_codec::decode_header(const std::vector<char>& buffer)
{
    memcpy(&header_, &buffer[0], sizeof(header_));
    decode_header_ = false;
    next_recv_bytes_ = sizeof(std::size_t) + sizeof(rpc_error_code) + 
        header_.message_name_len + header_.message_data_len;
}

void client_codec::decode_body(const std::vector<char>& buffer)
{
    std::size_t pos = 0;

    memcpy(&body_.serial_num, &buffer[pos], sizeof(body_.serial_num));
    pos += sizeof(body_.serial_num);

    memcpy(&body_.code, &buffer[pos], sizeof(body_.code));
    pos += sizeof(body_.code);

    body_.message_name.assign(&buffer[pos], header_.message_name_len);
    pos += header_.message_name_len;

    body_.message_data.assign(&buffer[pos], header_.message_data_len);

    reset();
    decode_data_callback_(body_);
}
