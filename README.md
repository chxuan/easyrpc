A modern RPC framework based on protobuf 
===============================================

> easyrpc是采用C++开发的，使用方便的RPC库。

![License][1] 

## Tutorial

* **Simple server**

    ```cpp
    #include "easyrpc/easyrpc.h"
    #include "common.pb.h"
    
    void echo(const std::shared_ptr<request>& req, const std::shared_ptr<response>& rsp)
    {
        rsp->set_response(req->message());
    }
    
    int main()
    {
        // 1.创建rpc服务器对象
        auto server = std::make_shared<rpc_server>();
        // 2.绑定echo函数
        // 0x0001为函数id
        server->register_handler(0x0001, std::bind(echo, 
                                                   std::placeholders::_1, std::placeholders::_2));
    
        // 3.配置监听参数并启动事件循环（非阻塞）
        // 服务端将采用4个io线程和4个work线程服务
        server->listen("0.0.0.0:8888").ios_threads(4).work_threads(4).run();

        std::cin.get();
        return 0;
    }
    ```
    
* **Simple client**
    ```cpp
    #include "easyrpc/easyrpc.h"
    #include "common.pb.h"
    
    int main()
    {   
        // 1.创建rpc客户端对象
        auto client = std::make_shared<rpc_client>();
        // 2.配置连接参数并启动事件循环（非阻塞）
        client->connect("127.0.0.1:8888").request_timeout(3).run();
    
        auto req = std::make_shared<echo_message>();
        req->set_echo_str("Hello world");
        req->set_echo_num(100);

        // 3.异步调用echo函数
        client->call(0x0001, message, [](const std::shared_ptr<result>& ret)
        {
            log_info() << ret->message()->DebugString();
        });
    
        std::cin.get();
        return 0;
    }
    ```

## 开发平台

* Ubuntu16.04 LTS gcc5.3.1

## 依赖性

* boost
* protobuf
* c++11

## DONE

* TCP长连接。
* 异步调用。
* 日志记录。
* worker线程池处理任务。
* 客户端超时处理。
* 支持发布/订阅模式。
* 数据重发功能。

## TODO

* 服务注册、发现。
* 支持HTTP/HTTPS协议。


## License
This software is licensed under the [MIT license][2]. © 2017 chxuan


  [1]: http://img.shields.io/badge/license-MIT-blue.svg?style=flat-square
  [2]: https://github.com/chxuan/easyrpc/blob/master/LICENSE
