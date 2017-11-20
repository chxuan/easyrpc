#include "client_codec.h"

client_codec::client_codec()
{
    reset();
}

client_codec::~client_codec()
{

}

std::shared_ptr<std::string> client_codec::encode(unsigned int serial_num, 
                                                  const std::string& func_name, 
                                                  const std::shared_ptr<google::protobuf::Message>& message)
{
    auto body = encode_body(serial_num, func_name, message);
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

void client_codec::set_decode_data_callback(const std::function<void(const response_content&)>& func)
{
    decode_data_callback_ = func;
}

request_header client_codec::encode_header(const request_content& body)
{
    request_header header;
    header.protocol_len = body.protocol.size();
    header.message_name_len = body.message_name.size();
    header.body_len = body.body.size();

    return header;
}

request_content client_codec::encode_body(unsigned int serial_num, 
                                          const std::string& func_name, 
                                          const std::shared_ptr<google::protobuf::Message>& message)
{
    client_flag flag{ serialize_mode::serialize, client_type::async_rpc_client };

    request_content body;
    body.call_id = serial_num;
    body.flag = flag;
    body.protocol = func_name;
    body.message_name = message->GetDescriptor()->full_name();
    body.body = protobuf_serialize::serialize(message);

    return body;
}

std::shared_ptr<std::string> client_codec::make_network_data(const request_header& header, const request_content& body)
{
    auto network_data = std::make_shared<std::string>();
    network_data->append(reinterpret_cast<const char*>(&header), sizeof(header));
    network_data->append(reinterpret_cast<const char*>(&body.call_id), sizeof(body.call_id));
    network_data->append(reinterpret_cast<const char*>(&body.flag), sizeof(body.flag));
    network_data->append(body.protocol);
    network_data->append(body.message_name);
    network_data->append(body.body);

    return network_data;
}

void client_codec::decode_header(const std::vector<char>& buffer)
{
    memcpy(&header_, &buffer[0], sizeof(header_));
    decode_header_ = false;
    next_recv_bytes_ = sizeof(unsigned int) + sizeof(rpc_error_code) 
        + header_.message_name_len + header_.body_len;
}

void client_codec::decode_body(const std::vector<char>& buffer)
{
    memcpy(&body_.call_id, &buffer[0], sizeof(body_.call_id));
    memcpy(&body_.code, &buffer[sizeof(body_.call_id)], sizeof(body_.code));
    body_.message_name.assign(&buffer[sizeof(body_.call_id) 
                                 + sizeof(body_.code)], header_.message_name_len);
    body_.body.assign(&buffer[sizeof(body_.call_id) + sizeof(body_.code) 
                         + header_.message_name_len], header_.body_len);

    reset();
    decode_data_callback_(body_);
}
