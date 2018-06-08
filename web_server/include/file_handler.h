#pragma once

#include <handler.h>

class FileHandler : public Handler {
    std::string filePath;
    std::string mimeType;
    std::string cache;
    bool binary;
public:
    /**
     *
     * @param route
     * @param filePath
     * @param mimeType
     * @param isBinary
     */
    FileHandler(const char * route, const char * filePath, const char * mimeType, bool isBinary);

    /**
     *
     */
    void exec();

    /**
     *
     * @param filePath
     * @param data
     * @return
     */
    static bool loadFile(const char * filePath, std::string & data);
};
