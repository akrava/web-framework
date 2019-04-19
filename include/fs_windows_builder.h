#pragma once
#if defined(_WIN32) || defined(_Win64)

#include "fs_builder.h"

/**
 * @brief Windows builder for fs
 */
class FsWindowsBuilder : public FsBuilder {
public:
    /**
     *
     * @param filePath
     * @return
     */
    FsFile * buildFile(std::string filePath) override;

    /**
     *
     * @param folderPath
     * @return
     */
    FsFolder * buildFolder(std::string folderPath, const char * folderName) override;
};

#endif