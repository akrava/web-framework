#pragma once

#include <string>
#include <fstream>

/**
 *
 */
class LogManager {
    std::string fileName;
    bool isOpened;
public:
    /**
     *
     * @param fileName
     */
    LogManager(const char * fileName);

    /**
     *
     * @param log
     * @param data
     * @return
     */
    void operator<<(const char * data);
    void operator<<(std::string data);
};
