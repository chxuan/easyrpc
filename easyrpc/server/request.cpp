#include "request.h"

request::request(const std::shared_ptr<google::protobuf::Message>& message, 
                 const std::string& session_id)
    : message_(message),
    session_id_(session_id)
{

}

std::shared_ptr<google::protobuf::Message> request::message() const
{
    return message_;
}

std::string request::session_id() const
{
    return session_id_;
}
