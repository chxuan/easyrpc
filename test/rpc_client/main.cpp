#include <iostream>
#include "easyrpc/easyrpc.h"
#include "../proto/code/proto_message.pb.h"

void session_status_callback(bool established, const std::string& session_id)
{
    if (established)
    {
        log_info() << "session established, session id: " << session_id;
    }
    else 
    {
        log_warn() << "session closed, session id: " << session_id;
    }
}

int main()
{   
    auto client = std::make_shared<rpc_client>();
    client->set_session_status_callback(std::bind(session_status_callback, 
                                                  std::placeholders::_1, std::placeholders::_2));
    bool success = client->connect("127.0.0.1:8888").connect_timeout(3).request_timeout(3).run();
    if (!success)
    {
        log_error() << "connect server failed";
        return 0;
    }

    auto message = std::make_shared<request_person_info_message>();
    message->set_name("Jack");
    message->set_age(20);

#if 1
    client->call(0x0002, message, [](const std::shared_ptr<result>& ret)
    {
        log_info() << ret->message()->DebugString();
    });
#else
    for (int i = 0; i < 10; ++i)
    {
        client->call(0x0001, message, [](const std::shared_ptr<result>& ret)
        {
            if (ret->error())
            {
                log_warn() << ret->error().message();
                return;
            }
            ret->message()->PrintDebugString();
        });
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
#endif

    std::cin.get();
    client->stop();

    return 0;
}

