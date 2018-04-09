#pragma once
#include <string>

class InitParams {
    bool run_app;
    bool ipv6;
    std::string ip;
    int port;
    std::string log_filename;
public:
    InitParams(int argc, char ** argv);
    bool getIpv6();
    std::string getIp();
    int getPort();
};


