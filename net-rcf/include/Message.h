/* Copyright(C)
* For free
* All right reserved
* 
*/
/**
* @file Message.h
* @brief RCF消息传输定义
* @author highway-9, 787280310@qq.com
* @version 1.1.0
* @date 2016-01-04
*/

#ifndef _MESSAGE_H
#define _MESSAGE_H

#include <string>
#include <RCF/RCF.hpp>

/**
* @brief RCF消息传输父类
*/
class Message
{
  public:
    /**
    * @brief Message 构造函数
    *
    * @param messageType 消息类型
    */
    Message(int messageType)
        : m_messageType(messageType)
    {
        // Do nothing
    }

    /**
    * @brief ~Message 析构函数
    */
    virtual ~Message()
    {
        // Do nothing    
    }

    /**
    * @brief serialize 序列化消息
    *
    * @param ar RCF自带消息序列化类型
    */
    void serialize(SF::Archive& ar)
    {
        ar & m_messageType;
    }

    /**
    * @brief registerType 注册类类型
    *
    * @tparam T 类类型
    * @param className 类名
    * @param classNameString 字符串类名
    * @note 该函数被继承了该类的类调用
    */
    template <typename T>
    void registerType(T className, const std::string& classNameString)
    {
        SF::registerType<className>(classNameString.c_str());
        SF::registerBaseAndDerived<Message, className>();
    }

  public:
    int m_messageType;          /// 消息类型
};

// RCF方法定义,其中第一个Message*是发送参数，第二个Message*是返回参数
RCF_BEGIN(I_RcfMessage, "I_RcfMessage")
    RCF_METHOD_V2(void, sendMesage, Mesage*, Message*)
RCF_END(I_RcfMessage)

#endif
