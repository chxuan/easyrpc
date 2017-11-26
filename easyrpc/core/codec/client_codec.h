/**
 * @file client_codec.h
 * @brief 客户端编解码器
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-10
 */
#pragma once

#include "easyrpc/core/codec/codec.h"

class tcp_session;
class result;

class client_codec : public codec
{
public:
    client_codec();
    virtual ~client_codec();

    virtual std::shared_ptr<std::string> encode(int serial_num, 
                                                int func_id,
                                                const std::shared_ptr<google::protobuf::Message>& message); 
    virtual void decode(const std::vector<char>& buffer, const std::shared_ptr<tcp_session>& session);
    virtual void reset();

private:
    request_header encode_header(const request_body& body);
    request_body encode_body(int serial_num, 
                             int func_id,
                             const std::shared_ptr<google::protobuf::Message>& message); 
    std::shared_ptr<std::string> make_network_data(const request_header& header, const request_body& body);
    void decode_header(const std::vector<char>& buffer);
    void decode_body(const std::vector<char>& buffer);
    std::shared_ptr<result> make_result();
    void prepare_decode_header();
    void prepare_decode_body();
    
private:
    response_header header_;
    response_body body_;
};
