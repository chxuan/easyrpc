#include "result.h"

result::result(int serial_num, const std::shared_ptr<google::protobuf::Message>& message) 
    : serial_num_(serial_num), 
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
