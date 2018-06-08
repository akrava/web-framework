#pragma once

#include <string>
#include <init_params.h>
#include <sys/socket.h>

/**
 *
 */
class Socket {
    sockaddr * socketAddress;
    int socket_fd;
    int client_fd;
    std::string ip;
    int port;
    bool ipv6;
public:
    /**
     *
     * @param params
     */
    Socket(InitParams params);

    /**
     *
     * @param ip
     * @param port
     * @param isIPv6
     */
    Socket(std::string ip, int port, bool isIPv6);
    ~Socket();

    /**
     *
     */
    void init();

    /**
     *
     * @return
     */
    std::string getData();

    /**
     *
     * @param data
     */
    void receiveData(std::string &data);

    std::string toString();
};
