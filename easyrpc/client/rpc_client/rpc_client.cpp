#include "rpc_client.h"
#include "easyrpc/utility/logger.h"
#include "easyrpc/core/codec/client_codec.h"
#include "easyrpc/client/rpc_client/task_dispatcher.h"

rpc_client::rpc_client()
{
    codec_ = std::make_shared<client_codec>();
}

rpc_client::~rpc_client()
{
    stop();
}

bool rpc_client::run()
{
    if (tcp_client::run())
    {
        dispatcher_ = std::make_shared<task_dispatcher>(request_timeout_);
        return true;
    }

    return false;
}

void rpc_client::stop()
{
    tcp_client::stop();
    if (dispatcher_)
    {
        dispatcher_->stop();
    }
}

auto rpc_client::make_result_handler(const std::function<void(const std::shared_ptr<result>&)>& func)
{
    auto recv_handler = [func](const std::shared_ptr<result>& ret)
    {
        try
        {
            func(ret);
        }
        catch (std::exception& e)
        {
            log_warn() << e.what();
        }
    };

    return recv_handler;
}

int rpc_client::call(int func_id,
                     const std::shared_ptr<google::protobuf::Message>& message, 
                     const std::function<void(const std::shared_ptr<result>&)>& func)
{
    int serial_num = make_serial_num();
    auto handler = make_result_handler(func);
    dispatcher_->add_result_handler(serial_num, handler);

    auto network_data = codec_->encode(serial_num, func_id, message);
    tcp_client::async_write(network_data);

    return serial_num;
}

int rpc_client::make_serial_num()
{
    static std::atomic<int> serial_num{ 0 };
    return ++serial_num;
}
