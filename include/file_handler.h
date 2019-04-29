#pragma once

#include "handler.h"

/**
 * @brief this class allow you to set any file of filesystem as response body
 *
 * FileHandler can handle as text files (like css, js), as binary data (img, png others)
 */
class FileHandler : public Handler {
    std::string filePath;
    std::string mimeType;
    std::string cache;
    bool binary;
public:
    /**
     * create file handlers with specified uri route, file path, content type etc.
     *
     * @param route
     *      uri route file
     * @param filePath
     *      local file path
     * @param mimeType
     *      content type
     * @param isBinary
     *      if file is binary, set true, false if it's text.
     */
    FileHandler(const char * route, const char * filePath, const char * mimeType, bool isBinary);

    /**
     * make http body of response object as file in filePath
     */
    void exec() override;

    /**
     * static function that read all data from file to string
     *
     * @param filePath
     *      path to file
     * @param data
     *      out param, if can read file, it will be written to data,do nothing otherwise
     * @return
     *      true, if read successfully, false otherwise
     */
    static bool loadFile(const char * filePath, std::string & data);
};
