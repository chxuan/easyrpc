/**
 * @file codec.h
 * @brief 编解码器基类
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-10
 */
#pragma once

#include <functional>
#include <memory>
#include <vector>
#include <google/protobuf/message.h>

class tcp_session;

struct packet_header
{
    int message_name_len;
    int message_data_len;
};

struct packet_body
{
    int serial_num;
    int func_id;
    std::string message_name;
    std::string message_data;
};

class codec
{
public:
    codec();
    virtual ~codec() = default;

    std::shared_ptr<std::string> encode(int serial_num, 
                                        int func_id, 
                                        const std::shared_ptr<google::protobuf::Message>& message);
    void decode(const std::vector<char>& buffer, const std::shared_ptr<tcp_session>& session);
    void reset();
    int get_next_recv_bytes();

protected:
    virtual void handle_decode_data(const packet_body& body, 
                                    const std::shared_ptr<tcp_session>& session) = 0;

private:
    packet_header encode_header(const packet_body& body);
    packet_body encode_body(int serial_num, 
                            int func_id, 
                            const std::shared_ptr<google::protobuf::Message>& message); 
    std::shared_ptr<std::string> make_network_data(const packet_header& header, const packet_body& body);

    void decode_header(const std::vector<char>& buffer);
    void decode_body(const std::vector<char>& buffer, const std::shared_ptr<tcp_session>& session);

    void prepare_decode_header();
    void prepare_decode_body();

    template<typename T>
    void copy_from_buffer(T& t, int& pos, const std::vector<char>& buffer);
    template<typename T>
    void copy_from_buffer(T& t, const std::vector<char>& buffer);
    void copy_from_buffer(std::string& str, int& pos, int len, const std::vector<char>& buffer);

    template<typename T>
    void copy_to_buffer(T& t, std::shared_ptr<std::string>& buffer);
    void copy_to_buffer(const std::string& str, std::shared_ptr<std::string>& buffer);

    bool is_vaild_header(int message_name_len, int message_data_len);

private:
    int next_recv_bytes_ = 0;
    bool decode_header_ = true;
    packet_header header_;
    packet_body body_;
};

template<typename T>
void codec::copy_from_buffer(T& t, int& pos, const std::vector<char>& buffer)
{
    memcpy(&t, &buffer[pos], sizeof(t));
    pos += sizeof(t);
}

template<typename T>
void codec::copy_from_buffer(T& t, const std::vector<char>& buffer)
{
    memcpy(&t, &buffer[0], sizeof(t));
}

template<typename T>
void codec::copy_to_buffer(T& t, std::shared_ptr<std::string>& buffer)
{
    buffer->append(reinterpret_cast<const char*>(&t), sizeof(t));
}

