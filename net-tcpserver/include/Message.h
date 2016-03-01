/* Copyright(C)
* For free
* All right reserved
*
*/
/**
* @file Message.h
* @brief 用于接收tcp消息
* @author highway-9, 787280310@qq.com
* @version 1.1.0
* @date 2016-02-03
*/

#ifndef _MESSAGE_H
#define _MESSAGE_H

#include <string>
#include <boost/smart_ptr.hpp>

#define TCP_MESSAGE_HEADER unsigned int m_messageType;

/**
* @brief 用于tcp服务器接收的消息结构
*/
class Message
{
public:
    TCP_MESSAGE_HEADER

    std::string m_data;
};

typedef boost::shared_ptr<Message> MessagePtr;

#endif
