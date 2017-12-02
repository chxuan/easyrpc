#include "rpc_client.h"
#include "easyrpc/utility/logger.h"
#include "easyrpc/codec/client_codec.h"
#include "easyrpc/client/task_dispatcher.h"

rpc_client::rpc_client()
{
    codec_ = std::make_shared<client_codec>();
    dispatcher_ = std::make_shared<task_dispatcher>();
}

rpc_client::~rpc_client()
{
    stop();
}

bool rpc_client::run()
{
    if (tcp_client::run())
    {
        dispatcher_->run(request_timeout_);
        return true;
    }

    return false;
}

void rpc_client::stop()
{
    tcp_client::stop();
    dispatcher_->stop();
}

int rpc_client::call(int func_id,
                     const std::shared_ptr<google::protobuf::Message>& message, 
                     const result_handler& handler)
{
    if (message)
    {
        int serial_num = make_serial_num();
        dispatcher_->add_result_handler(serial_num, handler);

        auto network_data = codec_->encode(serial_num, func_id, message);
        async_write(network_data);
        return serial_num;
    }

    return -1;
}

void rpc_client::register_handler(const sub_handler& handler)
{
    dispatcher_->register_handler(handler);
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
