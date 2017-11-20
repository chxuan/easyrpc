/**
 * @file client_codec.h
 * @brief 客户端编解码器
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-10
 */
#pragma once

#include "easyrpc/core/codec/codec.h"

class client_codec : public codec
{
public:
    client_codec();
    virtual ~client_codec();

    virtual std::shared_ptr<std::string> encode(int serial_num, 
                                                int func_id,
                                                const std::shared_ptr<google::protobuf::Message>& message); 
    virtual void decode(const std::vector<char>& buffer);
    virtual void reset();
    virtual void set_decode_data_callback(const std::function<void(const response_body&)>& func);

private:
    request_header encode_header(const request_body& body);
    request_body encode_body(int serial_num, 
                             int func_id,
                             const std::shared_ptr<google::protobuf::Message>& message); 
    std::shared_ptr<std::string> make_network_data(const request_header& header, const request_body& body);
    void decode_header(const std::vector<char>& buffer);
    void decode_body(const std::vector<char>& buffer);
    
private:
    response_header header_;
    response_body body_;
    std::function<void(const response_body&)> decode_data_callback_;
};
