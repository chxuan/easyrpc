#include "server_codec.h"
#include "easyrpc/core/protocol/sig.h"
#include "easyrpc/core/net/tcp_session.h"
#include "easyrpc/server/request.h"
#include "easyrpc/server/response.h"

void server_codec::handle_decode_data(const packet_body& body, 
                                      const std::shared_ptr<tcp_session>& session)
{

    auto req = std::make_shared<request>(protobuf_serialize::unserialize(body.message_name, body.message_data), 
                                         session->get_session_id());
    auto rsp = std::make_shared<response>(session, body.serial_num);
    emit complete_server_decode_data(body.func_id, req, rsp);
}
