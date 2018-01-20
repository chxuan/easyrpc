A modern RPC framework based on protobuf 
===============================================

> easyrpc是采用C++开发的，使用方便的RPC库。

![License][1] 

## Tutorial

* **Simple server**

    ```cpp
    #include "easyrpc/easyrpc.h"
    #include "common.pb.h"
    
    using namespace easyrpc;
    using namespace std::placeholders;

    void echo(const std::shared_ptr<request>& req, const std::shared_ptr<response>& res)
    {
        res->set_response(req->message);
    }
    
    int main()
    {
        // 1.创建rpc服务器对象
        // 服务端将采用1个io线程和2个work线程服务
        auto server = std::make_shared<rpc_server>("0.0.0.0:8888", 1, 2);

        // 2.设置路由
        server->route(echo_message::descriptor()->full_name(), std::bind(echo, _1, _2));
    
        // 3.启动事件循环（非阻塞）
        server->run();

        std::cin.get();
        return 0;
    }
    ```
    
* **Simple client**
    ```cpp
    #include "easyrpc/easyrpc.h"
    #include "common.pb.h"
    
    using namespace easyrpc;

    int main()
    {   
        // 1.创建rpc客户端对象
        // 配置连接地址并设置请求超时为3秒
        auto client = std::make_shared<rpc_client>("127.0.0.1:8888", 3);

        // 2.启动事件循环（非阻塞）
        client->run();
    
        auto req = std::make_shared<echo_message>();
        req->set_str("Hello world");
        req->set_num(1024);

        // 3.异步调用echo函数
        client->call(message, [](const std::shared_ptr<result>& ret)
        {
            log_info << ret->message->DebugString();
        });
    
        std::cin.get();
        return 0;
    }
    ```

## 开发平台

* Ubuntu17.10 gcc7.2.0

## 依赖性

* boost
* protobuf
* c++11

## DONE

* TCP长连接。
* rpc异步调用。
* 日志记录。
* worker线程池处理任务。
* 客户端请求超时处理。
* 支持主动推送模式。

## License
This software is licensed under the [MIT license][2]. © 2017 chxuan


  [1]: http://img.shields.io/badge/license-MIT-blue.svg?style=flat-square
  [2]: https://github.com/chxuan/easyrpc/blob/master/LICENSE
