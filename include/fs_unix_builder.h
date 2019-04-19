#pragma once
#ifdef __linux__
#include "fs_builder.h"

/**
 * @brief Unix builder for fs
 */
class FsUnixBuilder : public FsBuilder {
public:
    /**
     *
     * @param filePath
     * @return
     */
    FsFile * buildFile(std::string & filePath) override;

    /**
     *
     * @param folderPath
     * @return
     */
    FsFolder * buildFolder(std::string & folderPath, const char * folderName) override;
};

#endif