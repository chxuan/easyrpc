/**
 * @file server_codec.h
 * @brief 服务端编解码器
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-10
 */
#pragma once

#include "codec.h"

class request;
class response;

using request_handler = std::function<void(const std::shared_ptr<request>&, const std::shared_ptr<response>&)>;

class server_codec : public codec
{
public:
    server_codec(const request_handler& func);

protected:
    virtual void handle_decode_data(const packet_body& body, const std::shared_ptr<tcp_session>& session);

private:
    request_handler func_;
};
