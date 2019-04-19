#pragma once
#ifdef __linux__

#include <string>
#include <sys/socket.h>
#include "socket_api.h"

/**
 * @brief Unix realization for network
 */
class SocketUnixAPI : public SocketAPI {
    sockaddr * socketAddress;
    int socket_fd;
    int client_fd;
public:
    /**
     *
     */
    ~SocketUnixAPI() override;

    /**
     *
     */
    void init(std::string ip, int port, bool isIpv6) override ;

    /**
     *
     */
    std::string getData() override;

    /**
     *
     */
    void receiveData(const std::string & data) override;

	/**
	 *
	 */
	std::string getIpFromDomain(std::string &domain, bool isHttps, bool *IPv6) override;
};

#endif  // __linux__
