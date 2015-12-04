framework set
===================


目前该框架包括网络框架（thrift）、常用工具库、之后还会添加db操作库以及日志库。

----------


依赖的第三方库
-------------

> - net-thrift : boost、thrift.
> - utils : libuuid、openssl.

开发平台
-------------

该框架集合是在CentOS-7-x86_64-Minimal-1503-01下开发的，gcc版本是4.8.3。


开发平台搭建
-------------

将boost、thrift、libuuid、openssl库使用git clone到本地计算机，并将这些第三方库拷贝到/usr/local下，并在/etc/profile添加环境变量：

> **/etc/profile:**

> - export BOOST_INCLUDE_PATH="/usr/local/boost/include"
> - export BOOST_LIB_PATH="/usr/local/boost/lib"
> - export THRIFT_INCLUDE_PATH="/usr/local/thrift/include"
> - export THRIFT_LIB_PATH="/usr/local/thrift/lib"
> - export UUID_INCLUDE_PATH="/usr/local/uuid/include"
> - export UUID_LIB_PATH="/usr/local/uuid/lib"
> - export OPENSSL_INCLUDE_PATH="/usr/local/ssl/include"
> - export OPENSSL_LIB_PATH="/usr/local/ssl/lib"
> - 最后退出/etc/profile并执行source /etc/profile让更改立即生效。
