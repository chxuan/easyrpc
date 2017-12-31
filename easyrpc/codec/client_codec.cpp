#include "client_codec.h"
#include "easyrpc/utility/protobuf_serialize.h"
#include "easyrpc/client/result.h"

client_codec::client_codec(const result_handler& func)
    : func_(func)
{

}

void client_codec::handle_decode_data(const packet_body& body, const std::shared_ptr<tcp_session>& session)
{

    (void)session;
    auto ret = std::make_shared<result>(body.serial_num, 
                                        protobuf_serialize::unserialize(body.message_name, body.message_data));
    func_(ret);
}
