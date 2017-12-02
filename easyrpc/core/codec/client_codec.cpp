#include "client_codec.h"
#include "easyrpc/core/protocol/sig.h"
#include "easyrpc/client/result.h"

void client_codec::handle_decode_data(const packet_body& body, 
                                      const std::shared_ptr<tcp_session>& session)
{

    (void)session;
    auto ret = std::make_shared<result>(body.serial_num, 
                                        protobuf_serialize::unserialize(body.message_name, body.message_data));
    emit complete_client_decode_data(ret);
}
