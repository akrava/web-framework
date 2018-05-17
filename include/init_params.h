#pragma once
#include <string>

class InitParams {
    bool IPv6;
    int port;
    std::string host;
    std::string logFilePath;
public:
    InitParams();
    InitParams(int argc, char ** argv);
    bool isIPv6();
    const char * getIP();
    int getPort();
    std::string getFilePath();
};
