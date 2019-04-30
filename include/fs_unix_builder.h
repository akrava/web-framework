#pragma once
#ifdef __linux__
#include "fs_builder.h"

/**
 * @brief Concrete builder template for FsMiddleware for Unix OS
 *
 * This class is intended to create a file system hierarchy for specified
 *      folder by given path for Unix filesystem
 */
class FsUnixBuilder : public FsBuilder {
public:
    /**
     * make a FsComponent wrapper for files - leaves of hierarchy
     *
     * @param filePath
     *      full path to concrete file
     * @return
     *      FsFile object
     */
    FsFile * buildFile(std::string & filePath) override;

    /**
     * make a FsComponent wrapper for folder, and then recursively
     *      build hierarchy from inner folder and files
     *
     * @param folderPath
     *      path to folder
     * @param folderName
     *      name of folder
     * @return
     *      FsFolder object
     */
    FsFolder * buildFolder(std::string & folderPath, const char * folderName) override;
};

#endif