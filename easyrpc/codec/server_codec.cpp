#include "server_codec.h"
#include "easyrpc/utility/protobuf_serialize.h"
#include "easyrpc/net/tcp_session.h"
#include "easyrpc/server/request.h"
#include "easyrpc/server/response.h"

namespace easyrpc
{

server_codec::server_codec(const request_handler& func)
    : func_(func)
{

}

void server_codec::deal_decode_data(const packet_body& body, const std::shared_ptr<tcp_session>& session)
{
    auto message = protobuf_serialize::unserialize(body.message_name, body.message_data);
    if (message)
    {
        auto req = std::make_shared<request>(message, session->get_session_id());
        auto res = std::make_shared<response>(session, body.serial_num);
        func_(req, res);
    }
}

}
