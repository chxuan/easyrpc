#include "server_codec.h"
#include "easyrpc/utility/logger.h"
#include "easyrpc/core/protocol/sig.h"
#include "easyrpc/core/net/tcp_session.h"
#include "easyrpc/server/request.h"
#include "easyrpc/server/response.h"

server_codec::server_codec()
{
    reset();
}

server_codec::~server_codec()
{

}

std::shared_ptr<std::string> server_codec::encode(int serial_num, 
                                                  const std::shared_ptr<google::protobuf::Message>& message)
{
    auto body = encode_body(serial_num, message);
    auto header = encode_header(body);
    return make_network_data(header, body);
}

void server_codec::decode(const std::vector<char>& buffer, const std::shared_ptr<tcp_session>& session)
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

void server_codec::reset()
{
    prepare_decode_header();
}

response_header server_codec::encode_header(const response_body& body)
{
    response_header header;
    header.message_name_len = body.message_name.size();
    header.message_data_len = body.message_data.size();

    return header;
}

response_body server_codec::encode_body(int serial_num, 
                                        const std::shared_ptr<google::protobuf::Message>& message)
{
    response_body body;
    body.serial_num = serial_num;
    body.message_name = message->GetDescriptor()->full_name();
    body.message_data = protobuf_serialize::serialize(message);

    return body;
}

std::shared_ptr<std::string> server_codec::make_network_data(const response_header& header, const response_body& body)
{
    auto network_data = std::make_shared<std::string>();

    copy_to_buffer(header, network_data);
    copy_to_buffer(body.serial_num, network_data);
    copy_to_buffer(body.message_name, network_data);
    copy_to_buffer(body.message_data, network_data);

    return network_data;
}

void server_codec::decode_header(const std::vector<char>& buffer)
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

void server_codec::decode_body(const std::vector<char>& buffer, const std::shared_ptr<tcp_session>& session)
{
    int pos = 0;

    copy_from_buffer(body_.serial_num, pos, buffer);
    copy_from_buffer(body_.func_id, pos, buffer);
    copy_from_buffer(body_.message_name, pos, header_.message_name_len, buffer);
    copy_from_buffer(body_.message_data, pos, header_.message_data_len, buffer);

    prepare_decode_header();

    auto req = std::make_shared<request>(protobuf_serialize::unserialize(body_.message_name, body_.message_data), 
                                         session->get_session_id());
    auto rsp = std::make_shared<response>(session, body_.serial_num);
    emit complete_server_decode_data(body_.func_id, req, rsp);
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
