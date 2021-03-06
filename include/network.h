#pragma once

#include "socket_api.h"
#include "init_params.h"

/**
 * @brief Network bridge pattern
 */
class Network {
    std::string ip;
    int port;
    bool isIpv6;
protected:
    SocketAPI * socketImpl;
public:
    /**
     *
     */
    Network(const char * ip, int port, bool isIPv6);

    /**
     *
     * @param params
     */
    explicit Network(InitParams params);

    /**
     *
     * @param api
     */
    void setAPI(SocketAPI * api);

    /**
     *
     * @return
     */
    bool init();

    /**
     *
     * @return
     */
    std::string readData();


    /**
     *
     * @param data
     * @return
     */
    bool receiveData(const std::string & data);

    /**
     *
     */
    std::string toString();
	
	/**
	 *
	 */
	static std::string getIpFromDomain(std::string &domain, bool isHttps, bool *IPv6);

	/**
	 *
	 */
	static SocketAPI * createNewSocket();
};