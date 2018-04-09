#include <init_params.h>
#include <sockets.h>

int main (int argc, char ** argv) {
    // todo, todo and todo
    // only for example
    InitParams cur = InitParams(argc, argv);
    Sockets socket = Sockets(cur);
    socket.launch_demo_v01();
    return 0;
}
