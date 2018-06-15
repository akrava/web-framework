#pragma once

#include <string>
#include <fstream>

/**
 * @brief logging info into file
 *
 * LogManager create file and append it with input data data
 */
class LogManager {
    std::string fileName;
    bool isOpened;
public:
    /**
     * create log file, if fileName is null no data will be written
     *
     * @param fileName
     *      path to file (could be null)
     */
    LogManager(const char * fileName);

    /**
     * append to log new info
     *
     * @param data
     *      logging information
     */
    void operator<<(const char * data);

    /**
     * append to log new info
     *
     * @param data
     *      logging information
     */
    void operator<<(std::string data);
};
