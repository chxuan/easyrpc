#include "router.h"
#include "easyrpc/core/protocol/sig.h"
#include "easyrpc/core/net/tcp_session.h"
#include <iostream>

router::router()
{
    qt_connect(complete_server_decode_data, std::bind(&router::handle_complete_server_decode_data,
                                                      this, std::placeholders::_1, std::placeholders::_2));
}

void router::bind(int func_id, const function_t& func)
{
    std::lock_guard<std::mutex> lock(mutex_);
    route_table_.emplace(func_id, func);
}

void router::handle_complete_server_decode_data(const request_body& body, 
                                                const std::shared_ptr<tcp_session>& session)
{
    std::cout << "serial_num: " << body.serial_num << std::endl;
    std::cout << "func_id: " << body.func_id << std::endl;
    std::cout << "message_name: " << body.message_name << std::endl;
    std::cout << "message_data_len: " << body.message_data.size() << std::endl;
    std::cout << "session id: " << session->get_session_id() << std::endl;
}

