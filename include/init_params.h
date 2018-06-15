#pragma once

#include <string>

/**
 * @brief InitParams is intended to get web-server configs from command line arguments
 *
 * This class make verification of ip-address, port etc...
 */
class InitParams {
    bool IPv6;
    int port;
    std::string host;
    std::string logFilePath;
public:
    /**
     * Create empty object
     */
    InitParams();

    /**
     * Get params from command line arguments
     *
     * @param argc
     *      num of params
     * @param argv
     *      params
     */
    InitParams(int argc, char ** argv);

    /**
     * check if ip address is IPv6
     *
     * @return
     *      true if ip address is IPv6, false otherwise
     */
    bool isIPv6();

    /**
     * get ip address
     *
     * @return
     *      ip host address
     */
    const char * getIP();

    /**
     * get port
     *
     * @return
     *      host post
     */
    int getPort();

    /**
     * get log file path
     *
     * @return
     *      log file path
     */
    std::string getFilePath();
};
