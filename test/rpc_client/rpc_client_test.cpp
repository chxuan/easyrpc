#include "rpc_client_test.h"

using namespace std::placeholders;

rpc_client_test::rpc_client_test()
{
    client_ = std::make_shared<rpc_client>("127.0.0.1:8888", 3);
    client_->set_connection_notify(std::bind(&rpc_client_test::deal_connection_notify, this, _1, _2));
    client_->bind(std::bind(&rpc_client_test::deal_sub_message, this, _1));
}

rpc_client_test::~rpc_client_test()
{
    stop();
}

void rpc_client_test::run()
{
    bool ok = client_->run();
    if (!ok)
    {
        log_error << "Connect server failed";
        return;
    }

    call();
}

void rpc_client_test::stop()
{
    client_->stop();
}

void rpc_client_test::deal_connection_notify(bool created, const std::string& session_id)
{
    if (created)
    {
        log_info << "Connection created: " << session_id;
    }
    else 
    {
        log_warn << "Connection closed: " << session_id;
    }
}

void rpc_client_test::deal_sub_message(const std::shared_ptr<result>& ret)
{
    log_info << ret->message->DebugString();
}

void rpc_client_test::call()
{
    client_->call(make_echo_message(), [](const std::shared_ptr<result>& ret)
    {
        log_info << ret->message->DebugString();
    });
}

std::shared_ptr<google::protobuf::Message> rpc_client_test::make_echo_message()
{
    auto message = std::make_shared<echo_message>();
    message->set_str("Hello world");
    message->set_num(1024);

    return message;
}
