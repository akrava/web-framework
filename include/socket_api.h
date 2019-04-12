#pragma once

/**
 *
 */
class SocketAPI {
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
};
