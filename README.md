framework set
===================


目前该框架集合包括日志框架、网络框架、常用工具库，之后还会添加db框架、消息队列框架等。

----------


依赖的第三方库
-------------

> - clog框架依赖log4cpp、utils
> - net-thrift框架依赖thrift、boost
> - net-rcf框架依赖RCF、boost
> - utils框架依赖libuuid、openssl

开发平台
-------------

该框架集合是在CentOS-7-x86_64下开发的，CMake版本是3.4.1，gcc版本是4.8.x。


开发平台搭建
-------------

[developmentkit](https://github.com/chxuan/developmentkit)是该框架集合所依赖的三方库以及三方库的源代码仓库，使用git将[developmentkit](https://github.com/chxuan/developmentkit)克隆到本地计算机，并将这些三方库拷贝到/usr/local下，并在/etc/bashrc添加环境变量。

> **/etc/bashrc:**

> - export BOOST_INCLUDE_PATH="/usr/local/3rdParty/boost/include"
> - export BOOST_LIB_PATH="/usr/local/3rdParty/boost/lib"
> - export THRIFT_INCLUDE_PATH="/usr/local/3rdParty/thrift/include"
> - export THRIFT_LIB_PATH="/usr/local/3rdParty/thrift/lib"
> - export UUID_INCLUDE_PATH="/usr/local/3rdParty/uuid/include"
> - export UUID_LIB_PATH="/usr/local/3rdParty/uuid/lib"
> - export OPENSSL_INCLUDE_PATH="/usr/local/3rdParty/ssl/include"
> - export OPENSSL_LIB_PATH="/usr/local/3rdParty/ssl/lib"
> - export LOG4CPP_INCLUDE_PATH="/usr/local/3rdParty/log4cpp/include"
> - export LOG4CPP_LIB_PATH="/usr/local/3rdParty/log4cpp/lib"
> - export RCF_INCLUDE_PATH="/usr/local/3rdParty/RCF/include"
> - export RCF_LIB_PATH="/usr/local/3rdParty/RCF/lib"
> - export LD_LIBRARY_PATH="$RCF_LIB_PATH:$LOG4CPP_LIB_PATH:$OPENSSL_LIB_PATH:$UUID_LIB_PATH:$THRIFT_LIB_PATH:$BOOST_LIB_PATH:$LD_LIBRARY_PATH"

编辑完/etc/bashrc后执行以下命令，让更改立即生效。
> sudo source /etc/bashrc


编译framework
-------------

> - cd framework
> - cmake .
> - make
> - make install

其中make将会自动编译出静态库和动态库并输出到各自项目的lib文件夹下，测试程序（例子程序）的可执行文件会输出到bin文件夹下，make install默认将编译好的静态库、动态库以及头文件安装到/usr/local/framework/目录下，编译好framework后，需要设置环境变量以便后续使用framework进行开发。


使用framework
-------------

###1.使用clog
```
//main.cpp
#include <iostream>
#include <string>
#include "LogWrapper.h"

int main()
{
    int a = 1;
    int b = 2;
    std::string str = "Hello world";

    // C风格
    logError("Error log");
    logWarn("Warn log");
    logInfo("a + b = %d", a + b);

    // C++风格
    logError() << "Hello world";
    logWarn() << "Hello world";
    logDebug() << "message: " << str;

    return 0;
}
```
正如你所看到的，使用clog提供的API很简单，有C风格和C++风格格式化输出，clog会同时将日志输出到屏幕和日志文件(logs/exename.log)，下面是输出日志。
```
2016-01-24 14:35:27,181: [ERROR] : main.cpp main(19) Error log
2016-01-24 14:35:27,181: [WARN ] : main.cpp main(20) Warn log
2016-01-24 14:35:27,181: [INFO ] : main.cpp main(21) a + b = 3
2016-01-24 14:35:27,182: [ERROR] : main.cpp main(24) Hello world
2016-01-24 14:35:27,182: [WARN ] : main.cpp main(25) Hello world
2016-01-24 14:35:27,182: [DEBUG] : main.cpp main(26) message: Hello world
```



