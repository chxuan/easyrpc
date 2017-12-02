#include "rpc_server_test.h"
#include <iostream>

int main()
{
    rpc_server_test test;
    test.run();

    std::cin.get();
    test.stop();

    return 0;
}

