/**
 * @file router.h
 * @brief rpc服务消息路由器
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-19
 */
#pragma once

#include <unordered_map>
#include <mutex>
#include <functional>
#include <memory>
#include "easyrpc/utility/qt_connect.h"
#include "easyrpc/core/protocol/protocol_define.h"

class request;
class response;
class tcp_session;

using function_t = std::function<void(const std::shared_ptr<request>&, 
                                      const std::shared_ptr<response>&)>;

class router
{
public:
    router();
    void bind(int func_id, const function_t& func);

private slots:
    void handle_complete_server_decode_data(const request_body& body, 
                                            const std::shared_ptr<tcp_session>& session);

private:
    std::unordered_map<int, function_t> route_table_;
    std::mutex mutex_;
};
