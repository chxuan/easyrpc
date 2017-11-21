#include "easyrpc/easyrpc.h"

int main()
{
    auto server = std::make_shared<rpc_server>();
    server->listen("0.0.0.0:8888").run();
    log_info() << "rpc server start...";

    std::cin.get();
    return 0;
}

