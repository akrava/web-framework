#pragma once

#include <string>
#include <init_params.h>

class Sockets {
    bool ipv6;
    std::string ip;
    int port;
public:
    Sockets(InitParams params);
    Sockets(bool ipv6, std::string ip, int port);
    void launch_demo_v01();
};



