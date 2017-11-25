#include "request.h"

request::request(const std::shared_ptr<google::protobuf::Message>& message, 
                 const std::string& session_id)
    : message_(message),
    session_id_(session_id)
{

}

std::shared_ptr<google::protobuf::Message> request::get_message() 
{
    return message_;
}

std::string request::get_session_id()
{
    return session_id_;
}
