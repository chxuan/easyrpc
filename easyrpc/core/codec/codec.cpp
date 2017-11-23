#include "codec.h"

codec::~codec()
{

}

std::shared_ptr<std::string> codec::encode(int serial_num, 
                                           int func_id,
                                           const std::shared_ptr<google::protobuf::Message>& message) 
{
    (void)serial_num;
    (void)func_id,
    (void)message;
    return nullptr;
}

void codec::set_decode_data_callback(const std::function<void(const response_body&)>& func)
{
    (void)func;
}

std::size_t codec::get_next_recv_bytes()
{
    return next_recv_bytes_;
}

void codec::copy_from_buffer(std::string& str, int& pos, int len, const std::vector<char>& buffer)
{
    str.assign(&buffer[pos], len);
    pos += len;
}

void codec::copy_to_buffer(const std::string& str, std::shared_ptr<std::string>& buffer)
{
    buffer->append(str);
}
