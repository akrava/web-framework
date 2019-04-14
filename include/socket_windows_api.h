#pragma once
#if defined(_WIN32) || defined(_Win64)

#include <string>
#include <winsock2.h>
#include "socket_api.h"

class SocketWindowsAPI : public SocketAPI {
	SOCKET serverSocket;
	SOCKET clientSocket;
public:
	/**
	 *
	 */
	~SocketWindowsAPI() override;

	/**
	 *
	 */
	void init(std::string ip, int port, bool isIpv6) override;

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

#endif // defined(_WIN32) || defined(_Win64)
