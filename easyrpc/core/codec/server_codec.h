/**
 * @file server_codec.h
 * @brief 服务端编解码器
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-10
 */
#pragma once

#include "easyrpc/core/codec/codec.h"

class tcp_session;

class server_codec : public codec
{
public:
    server_codec();
    virtual ~server_codec();

    virtual std::shared_ptr<std::string> encode(int serial_num, 
                                                const std::shared_ptr<google::protobuf::Message>& message);
    virtual void decode(const std::vector<char>& buffer, const std::shared_ptr<tcp_session>& session);
    virtual void reset();

private:
    response_header encode_header(const response_body& body);
    response_body encode_body(int serial_num, 
                              const std::shared_ptr<google::protobuf::Message>& message); 
    std::shared_ptr<std::string> make_network_data(const response_header& header, const response_body& body);
    void decode_header(const std::vector<char>& buffer);
    void decode_body(const std::vector<char>& buffer, const std::shared_ptr<tcp_session>& session);
    void prepare_decode_header();
    void prepare_decode_body();

private:
    request_header header_;
    request_body body_;
};
