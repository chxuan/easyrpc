#include "response.h"
#include "easyrpc/core/codec/codec.h"
#include "easyrpc/core/net/tcp_session.h"

response::response(const std::shared_ptr<tcp_session>& session, int serial_num)
    : session_(session),
    serial_num_(serial_num)
{

}

void response::set_response(const std::shared_ptr<google::protobuf::Message>& message)
{
    if (message)
    {
        auto network_data = session_->get_codec()->encode(serial_num_, rpc_error_code::ok, message);
        session_->async_write(network_data);
    }
}
