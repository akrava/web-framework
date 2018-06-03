#pragma once

#include <handler.h>

class FileHandler : public Handler {
    std::string filePath;
    std::string mimeType;
    std::string cache;
    bool binary;
public:
    FileHandler(const char * route, const char * filePath, const char * mimeType, bool isBinary);
    void exec();
    static bool loadFile(const char * filePath, std::string & data);
};
