#pragma once
#include <string>
#include <init_params.h>
#include <sys/socket.h>

class Socket {
    sockaddr * socketAddress;
    int socket_fd;
    int client_fd;
    std::string ip;
    int port;
    bool ipv6;
public:
    Socket(InitParams params);
    Socket(std::string ip, int port, bool isIPv6);
    ~Socket();
    void init();
    std::string getData();
    void reciveData(std::string & data);
};
