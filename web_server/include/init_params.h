#pragma once

#include <string>

/**
 *
 */
class InitParams {
    bool IPv6;
    int port;
    std::string host;
    std::string logFilePath;
public:
    /**
     *
     */
    InitParams();

    /**
     *
     * @param argc
     * @param argv
     */
    InitParams(int argc, char ** argv);

    /**
     *
     * @return
     */
    bool isIPv6();

    /**
     *
     * @return
     */
    const char * getIP();

    /**
     *
     * @return
     */
    int getPort();

    /**
     *
     * @return
     */
    std::string getFilePath();
};
