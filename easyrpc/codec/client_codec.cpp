#include "client_codec.h"
#include "easyrpc/utility/protobuf_serialize.h"
#include "easyrpc/client/result.h"

namespace easyrpc
{

client_codec::client_codec(const result_handler& func)
    : func_(func)
{

}

void client_codec::deal_decode_data(const packet_body& body, const std::shared_ptr<tcp_session>& session)
{

    (void)session;
    auto message = protobuf_serialize::unserialize(body.message_name, body.message_data);
    if (message)
    {
        auto ret = std::make_shared<result>(body.serial_num, body.model, message);
        func_(ret);
    }
}

}
