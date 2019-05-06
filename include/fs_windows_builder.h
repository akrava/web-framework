#pragma once
#if defined(_WIN32) || defined(_Win64)
#include "fs_builder.h"

/**
 * @brief Concrete builder template for FsMiddleware for Windows OS
 *
 * This class is intended to create a file system hierarchy for specified
 *      folder by given path for Windows filesystem
 */
class FsWindowsBuilder : public FsBuilder {
public:
    /**
      * make a FsComponent wrapper for files - leaves of hierarchy
     *
     * @param filePath
     *      full path to concrete file
     * @return
     *      FsFile object
     */
	FsFile * buildFile(std::string & filePath);

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