#pragma once

#include <string>
#include "init_params.h"
#include <sys/socket.h>

/**
 * @brief wrapper functions to send/receive data via web-sockets
 *
 * Implementation of sockets function for Linux OS
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
     * Filling object with host address information represented by InitParams object
     *
     * @param params
     *      object of class InitParams with info about host address
     */
    explicit Socket(InitParams params);

    /**
     * Filling object with host address information to do method init in future
     *
     * @param ip
     *      host address ip (IPv4 or IPv6)
     *
     * @param port
     *      host address port
     *
     * @param isIPv6
     *      if argument ip is version 6 set true, false otherwise
     */
    Socket(const char * ip, int port, bool isIPv6);

    /**
     * closing opened host socket
     */
    ~Socket();

    /**
     * create socket, binding created socket to host address and start listening port
     *
     * @throw
     *      RuntimeException when address was invalid, busy etc.
     */
    void init();

    /**
     * Accepting all clients at configured host and reading data from current client
     *
     * @throw
     *      RuntimeException when got error with reading from client
     *
     * @return
     *      data as string
     */
    std::string getData();

    /**
     * receive data to client, which sent data before it (method getData was used)
     *
     * @param data
     *      serialized data into string
     */
    void receiveData(std::string & data);

    /**
     * get information about current ip and port of host
     *
     * @return
     *      string in format "ip: xxx.xxx.xxx.xxx port: xx"
     */
    std::string toString();
};
