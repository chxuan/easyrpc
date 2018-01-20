#include "rpc_client.h"
#include "easyrpc/utility/logger.h"
#include "easyrpc/codec/client_codec.h"
#include "task_dispatcher.h"

namespace easyrpc
{

rpc_client::rpc_client(const std::string& address, int request_timeout)
    : tcp_client(address)
{
    dispatcher_ = std::make_shared<task_dispatcher>(request_timeout);
    codec_ = std::make_shared<client_codec>(std::bind(&task_dispatcher::dispatch, dispatcher_, std::placeholders::_1));
}

rpc_client::~rpc_client()
{
    stop();
}

bool rpc_client::run()
{
    if (tcp_client::run())
    {
        dispatcher_->run();
        return true;
    }

    return false;
}

void rpc_client::stop()
{
    tcp_client::stop();
    dispatcher_->stop();
}

int rpc_client::call(const std::shared_ptr<google::protobuf::Message>& message, const result_handler& handler)
{
    if (message)
    {
        int serial_num = make_serial_num();
        dispatcher_->add_result_handler(serial_num, handler);

        auto network_data = codec_->encode(serial_num, message_model::rpc, message);
        if (network_data)
        {
            async_write(network_data);
            return serial_num;
        }
    }

    return -1;
}

void rpc_client::bind(const sub_handler& handler)
{
    dispatcher_->bind(handler);
}

int rpc_client::make_serial_num()
{
    static std::atomic<int> serial_num{ -1 };
    if (++serial_num < 0)
    {
        serial_num = 0; 
    }

    return serial_num;
}

}
