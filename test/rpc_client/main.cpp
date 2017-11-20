#include <iostream>
#include "crpc/crpc.h"
#include "../proto/code/proto_message.pb.h"

void session_status_callback(session_status status, const std::string& session_id)
{
    if (status == session_status::established)
    {
        log_info() << "session established, session id: " << session_id;
    }
    else if (status == session_status::closed)
    {
        log_info() << "session closed, session id: " << session_id;
    }
}

int main()
{   
    auto client = std::make_shared<rpc_client>();
    client->set_session_status_callback(std::bind(session_status_callback, 
                                                  std::placeholders::_1, std::placeholders::_2));
    bool success = client->connect("127.0.0.1:50051").connect_timeout(3).request_timeout(10).run();
    if (!success)
    {
        log_error() << "connect server failed";
        return 0;
    }

    auto message = std::make_shared<request_person_info_message>();
    message->set_name("Jack");
    message->set_age(20);

#if 1
    client->call("request_person_info", message, [](const std::shared_ptr<result>& ret)
    {
        if (ret->error())
        {
            log_warn() << ret->error().message();
            return;
        }
        ret->message()->PrintDebugString();
    });
#else
    for (int i = 0; i < 10; ++i)
    {
        client->call("request_person_info", message, [](const std::shared_ptr<result>& ret)
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

