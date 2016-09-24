A RPC framework written in Modern C++
===============================================

> easyrpc是一个纯c++14开发，header-only，使用方便的RPC库，灵感来自于[topcpporg/rest_rpc][1]。

![License][2] 

## Getting started
首先下载easyrpc：

    git clone https://github.com/chxuan/easyrpc.git

然后下载依赖的第三方库：

    git submodule update --init --recursive
    
下载过后包含easyrpc头文件即可使用，在编译时需要指定序列化框架，添加`ENABLE_BOOST_SERIALIZATION`宏定义来启用boost.serialization序列化框架，添加`ENABLE_MSGPACK`宏定义来启用msgpack序列化框架，添加`ENABLE_JSON`宏定义来启用json序列化框架。

## easyrpc流程图
![此处输入图片的描述][9]

## Tutorial

* **Simple server**

    ```cpp
    #include <easyrpc/easyrpc.hpp>
    
    std::string echo(const std::string& str)
    {
        return str;
    }
    
    class utils
    {
    public:
        int add(int a, int b)
        {
            return a + b;
        }
    };
    
    int main()
    {
        easyrpc::server app;
        utils u;
        app.bind("say_hello", []
    	{ 
    		std::cout << "Hello" << std::endl;
    	});
    	app.bind("echo", &echo);
    	app.bind("add", &utils::add, &u);
    	
        app.listen(50051).timeout(3000).multithreaded(10).run();
        
        std::cin.get();
        return 0;
    }
    ```
    
    服务器调用bind函数绑定handler，支持成员函数、非成员函数以及lambda表达式的绑定，设置3000ms读socket超时（默认为永不超时），启用10个Worker线程处理业务（默认为单线程），内部IO线程使用`an io_service-per-CPU`（一个ioservice对应一个线程）模式，最大限度提升IO性能。
    
* **Simple client**
    ```cpp
    #include <easyrpc/easyrpc.hpp>
    
    EASYRPC_RPC_PROTOCOL_DEFINE(say_hello, void());
    EASYRPC_RPC_PROTOCOL_DEFINE(echo, std::string(const std::string&));
    EASYRPC_RPC_PROTOCOL_DEFINE(add, int(int, int));
    
    int main()
    {
        easyrpc::client app;
        app.connect("localhost:50051").timeout(5000).run();
    	app.call(say_hello);
    	auto str = app.call(echo, "Hello world");
    	auto ret = app.call(add, 1, 2);
    
        return 0;
    }
    ```
    
正如你所看到的，客户端像调用本地函数一样就能够完成与服务端的通信，一切都那么简洁方便，easyrpc目前只支持短连接调用，短连接的好处就是不用担心各个server的启动顺序、调用方便以及不用维护心跳，由于每次call都会去connect，所以没有长连接高效，后期可能会考虑增加长连接call，easyrpc使用boost.asio来作为网络底层，效率自然高效，boost.serialization作为序列化框架，可以用类对象、STL（vector、map等）作为函数参数。

* **User-define classes**
    ```cpp
    struct person_info_req
    {
        int card_id;
        std::string name;
    
    #ifdef ENABLE_BOOST_SERIALIZATION
        template<class Archive>
        void serialize(Archive& ar, const unsigned int)
        {
            ar & card_id;
            ar & name;
        }
    #endif
    
    #ifdef ENABLE_MSGPACK
        MSGPACK_DEFINE(card_id, name);
    #endif
    
    #ifdef ENABLE_JSON
        META(card_id, name);
    #endif
    };
    
    struct person_info_res 
    {
        int card_id;
        std::string name;
        int age;
        std::string national;
        
    #ifdef ENABLE_BOOST_SERIALIZATION
        template<class Archive>
        void serialize(Archive& ar, const unsigned int)
        {
            ar & card_id;
            ar & name;
            ar & age;
            ar & national;
        }
    #endif
        
    #ifdef ENABLE_MSGPACK
        MSGPACK_DEFINE(card_id, name, age, national);
    #endif
    
    #ifdef ENABLE_JSON
        META(card_id, name, age, national);
    #endif
    };
    ```
    
可以看到，[boost序列化][3]、[msgpack][4]和[json序列化][5]不需要写IDL交互定义文件，也不会自动生成代码，比[protobuf][6]和[thrift][7]方便易用，更多User-define classes细节可以查看各自官网，若想扩展序列化方式，那怎么办呢？不用担心，easyrpc可以支持扩展序列化方式，这里以[kapok][8]作为示例。

* **Server**
    ```cpp
    #include <easyrpc/easyrpc.hpp>
    
    std::string call_person(const std::string& str)
    {
        std::cout << str << std::endl;
        DeSerializer dr;
        person_info_req req;
        dr.Parse(str);
        dr.Deserialize(req);
    
        person_info_res res;
        res.card_id = req.card_id;
        res.name = req.name;
        res.age = 20;
        res.national = "han";
        Serializer sr;
        sr.Serialize(res);
        return sr.GetString();
    }

    int main()
    {
        easyrpc::server app;
    	app.bind_raw("call_person", &call_person);
        app.listen(50051).timeout(3000).multithreaded(10).run();
        
        std::cin.get();
        return 0;
    }
    ```
    
* **Client**

    ```cpp
    #include <easyrpc/easyrpc.hpp>
    
    int main()
    {
        easyrpc::client app;
        app.connect("localhost:50051").timeout(5000).run();
        
        person_info_req req2 { 12345678, "Jack" };
        Serializer sr;
        sr.Serialize(req2);
        std::string str = app.call_raw<easyrpc::two_way>("call_person", sr.GetString());
        std::cout << str << std::endl;

        person_info_res res2;
        DeSerializer dr;
        dr.Parse(str);
        dr.Deserialize(res2);
    
        return 0;
    }
    ```
    
其中`call_raw<easyrpc::two_way>`表示一应一答，若无需应答则使用`call_raw<one_way>`，`call_raw`的作用就是将序列化的工作交给了用户，起到了扩展序列化。
    
## Warning

* 以上samples为了简洁，所以没有捕获异常，用户在使用easyrpc时需要捕获异常。
* 使用boost序列化库时，客户端和服务端序列化库的版本务必统一，要么使用32位的，要么使用64位，不然不能够通信，因为boost.serialization使用std::size_t来存储字节长度，std::size_t在32位下为unsigned int，在64位下为unsigned long。
* 不支持指针，仅支持栈对象序列化。
* 发送、接收buf大小为8MB（protocol + body），大于8MB程序将抛出异常，用户可更改发送、接收buf大小。
* bind/call和bind_raw/call_raw需要保持一致性。

## 开发平台

* Ubuntu16.04 LTS gcc5.3.1
* MSVC2015

## 依赖性

* [easypack][10]
* [spdlog][11]
* boost
* c++14

## DONE

* 短连接调用。
* 同步调用。
* TCP协议。
* worker线程池处理任务。
* 日志记录。
* 客户端、服务端超时处理。
* 支持多种序列化框架（boost序列化、msgpack和json）。
* 支持扩展序列化方式。

## TODO

* ~~增加扩展序列化方式~~。
* 增加长连接调用。
* 增加发布/订阅模式。
* 增加其他序列化框架和协议（~~json、msgpack~~等）。
* 服务注册、发现。
* 支持HTTP/HTTPS协议。
* 异步调用。


## License
This software is licensed under the [MIT license][12]. © 2016 chxuan


  [1]: https://github.com/topcpporg/rest_rpc
  [2]: http://img.shields.io/badge/license-MIT-blue.svg?style=flat-square
  [3]: http://www.boost.org/
  [4]: https://github.com/msgpack/msgpack-c
  [5]: https://github.com/qicosmos/Kapok
  [6]: https://github.com/google/protobuf
  [7]: https://github.com/apache/thrift
  [8]: https://github.com/qicosmos/Kapok
  [9]: https://raw.githubusercontent.com/chxuan/images/master/easyrpc/easyrpc.png
  [10]: https://github.com/chxuan/easypack
  [11]: https://github.com/gabime/spdlog
  [12]: https://github.com/chxuan/easyrpc/blob/master/LICENSE
