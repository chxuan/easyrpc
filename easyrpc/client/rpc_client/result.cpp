#include "result.h"

result::result(const error_code& ec, 
               int serial_num, 
               const std::shared_ptr<google::protobuf::Message>& message) 
    : ec_(ec), 
    serial_num_(serial_num), 
    message_(message) 
{

}

std::shared_ptr<google::protobuf::Message> result::message() const
{
    return message_;
}

int result::serial_num() const
{
    return serial_num_;
}

error_code result::error() const 
{
    return ec_; 
}
