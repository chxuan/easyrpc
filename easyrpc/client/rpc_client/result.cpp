#include "result.h"

result::result(const error_code& ec, 
               unsigned int call_id, 
               const std::shared_ptr<google::protobuf::Message>& message) 
    : ec_(ec), 
    call_id_(call_id), 
    message_(message) 
{

}

result::result(const error_code& ec, 
               unsigned int call_id, 
               const std::string& raw_data) 
    : ec_(ec), 
    call_id_(call_id), 
    raw_data_(raw_data)
{

}

std::shared_ptr<google::protobuf::Message> result::message() const
{
    return message_;
}

std::string result::raw_data() const
{
    return raw_data_;
}

unsigned int result::call_id() const
{
    return call_id_;
}

error_code result::error() const 
{
    return ec_; 
}
