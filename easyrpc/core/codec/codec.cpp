#include "codec.h"

codec::~codec()
{

}

std::shared_ptr<std::string> codec::encode(unsigned int serial_num, 
                                           const std::string& func_name, 
                                           const std::shared_ptr<google::protobuf::Message>& message) 
{
    (void)serial_num;
    (void)func_name;
    (void)message;
    return nullptr;
}

void codec::set_decode_data_callback(const std::function<void(const response_content&)>& func)
{
    (void)func;
}

std::size_t codec::get_next_recv_bytes()
{
    return next_recv_bytes_;
}
