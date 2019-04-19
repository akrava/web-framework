#pragma once

#include "fs_file.h"
#include "fs_folder.h"

/**
 * @brief Builder template for fs middleware
 */
class FsBuilder {
protected:
    FsComponent * root = nullptr;
public:
    /**
     *
     */
    virtual ~FsBuilder() = default;

    /**
     *
     * @param filePath
     * @return
     */
    virtual FsFile * buildFile(std::string & filePath) = 0;

    /**
     *
     * @param folderPath
     * @return
     */
    virtual FsFolder * buildFolder(std::string & folderPath, const char * folderName) = 0;

    /**
     *
     * @return
     */
    FsComponent * getResult() { return root; }

    /**
     *
     */
    void reset() { root = nullptr; }
};