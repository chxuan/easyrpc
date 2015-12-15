/* Copyright(C)
* For free
* All right reserved
* 
*/
/**
* @file ThriftServerWrapper.h
* @brief thrift服务器通信包装
* @author highway-9, 787280310@qq.com
* @version 1.1.0
* @date 2015-11-25
*/

#ifndef _THRIFTSERVERWRAPPER_H
#define _THRIFTSERVERWRAPPER_H

#include "Message.h"
#include <string>

class ThriftServerImpl;

///  消息回调函数定义
typedef void (*MESSAGE_CALLBACK) (Message* message, Message* retMessage);


/**
* @brief thrift服务器通信包装类
*/
class ThriftServerWrapper
{
  public:
    /**
    * @brief ThriftServerWrapper 构造函数
    */
    ThriftServerWrapper();

    /**
    * @brief ~ThriftServerWrapper 析构函数
    */
    ~ThriftServerWrapper();

    /**
    * @brief init 初始化thrift服务器端
    *
    * @param port 监听端口，默认为9090
    */
    void init(const unsigned int& port = 9090);

    /**
    * @brief start 开始服务器
    *
    * @return 成功返回true，否则返回false
    */
    bool start();

    /**
    * @brief stop 停止服务器
    *
    * @return 成功返回true，否则返回false
    */
    bool stop();

    /**
    * @brief deinit 反初始化，释放一些资源
    */
    void deinit();

    /**
    * @brief setMessageCallback 设置消息回调函数
    *
    * @param func 回调函数指针
    */
    void setMessageCallback(MESSAGE_CALLBACK func);

  private:
    ThriftServerImpl*           m_impl;     ///< thrift服务器实现类指针
};

#endif
