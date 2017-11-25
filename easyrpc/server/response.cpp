#include "response.h"

response::response(const std::shared_ptr<tcp_session>& session, int serial_num)
    : session_(session),
    serial_num_(serial_num)
{

}

void response::set_response(const std::shared_ptr<google::protobuf::Message>& message)
{
    if (message)
    {
        /* std::string message_name = message->GetDescriptor()->full_name(); */
        /* std::string body = serialize_util::singleton::get()->serialize(message); */
        /* if (!message_name.empty() && !body.empty()) */
        /* { */
        /*     connect_->async_write(response_content{ call_id_, rpc_error_code::ok, message_name, body }); */
        /* } */                    
    }
}
