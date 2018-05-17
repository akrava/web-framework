#include <init_params.h>
#include <sockets.h>
#include <iostream>
#include "runtime_exception.h"

int main (int argc, char ** argv) {
    InitParams cur;
    try {
        cur = InitParams(argc, argv);
    } catch (RuntimeException & err) {
        std::cerr << err.what() << std::endl;
        return EXIT_FAILURE;
    }
    Sockets socket = Sockets(cur);
    socket.launch_demo_v01();
    return 0;
}
