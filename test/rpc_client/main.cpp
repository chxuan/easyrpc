#include "rpc_client_test.h"

int main()
{   
    rpc_client_test test;
    test.run();

    std::cin.get();
    test.stop();
    
    return 0;
}

