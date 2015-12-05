framework set
===================


目前该框架集合包括网络框架、常用工具库，之后还会添加db框架、日志框架、消息队列框架等。

----------


依赖的第三方库
-------------

> - net-thrift框架依赖boost、thrift
> - utils框架依赖libuuid、openssl

开发平台
-------------

该框架集合是在CentOS-7-x86_64-Minimal-1503-01下开发的，gcc版本是4.8.3。


开发平台搭建
-------------

[developmentkit](https://github.com/chxuan/developmentkit)是该框架集合所依赖的三方库以及三方库的源代码仓库，使用git将[developmentkit](https://github.com/chxuan/developmentkit)克隆到本地计算机，并将这些三方库拷贝到/usr/local下，并在/etc/profile添加环境变量。

> **/etc/profile:**

> - export BOOST_INCLUDE_PATH="/usr/local/boost/include"
> - export BOOST_LIB_PATH="/usr/local/boost/lib"
> - export THRIFT_INCLUDE_PATH="/usr/local/thrift/include"
> - export THRIFT_LIB_PATH="/usr/local/thrift/lib"
> - export UUID_INCLUDE_PATH="/usr/local/uuid/include"
> - export UUID_LIB_PATH="/usr/local/uuid/lib"
> - export OPENSSL_INCLUDE_PATH="/usr/local/ssl/include"
> - export OPENSSL_LIB_PATH="/usr/local/ssl/lib"
> - export LD_LIBRARY_PATH="\$OPENSSL_LIB_PATH:\$UUID_LIB_PATH:\$THRIFT_LIB_PATH:\$BOOST_LIB_PATH:\$LD_LIBRARY_PATH"
> - 最后退出/etc/profile并执行source /etc/profile让更改立即生效。
