#pragma once

/**
 *
 */
class SocketAPI {
	static SocketAPI * instance;
public:
    /**
     *
     */
    virtual ~SocketAPI() = default;

    /**
     *
     */
    virtual void init(std::string ip, int port, bool isIpv6) = 0;

    /**
     *
     */
    virtual std::string getData() = 0;

    /**
     *
     */
    virtual void receiveData(const std::string & data) = 0;

	/**
	 *
	 */
	virtual std::string getIpFromDomain(std::string &domain, bool isHttps, bool *IPv6) = 0;
};
