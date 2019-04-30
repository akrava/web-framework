#pragma once

#include "fs_file.h"
#include "fs_folder.h"

/**
 * @brief Builder template for FsMiddleware
 *
 * This class is intended to create a file system hierarchy for specified
 *      folder by given path. FsBuilder as a builder template make hierarchy
 *      for composite template, that represents file system: FsComponent,
 *      FsFile,FsFolder. By default, there are two builders: FsUnixBuilder
 *      and FsWindowsBuilder.
 */
class FsBuilder {
protected:
    FsComponent * root = nullptr;
public:
    /**
     * virtual destructor
     */
    virtual ~FsBuilder() = default;

    /**
     * make a FsComponent wrapper for files - leaves of hierarchy
     *
     * @param filePath
     *      full path to concrete file
     * @return
     *      FsFile object
     */
    virtual FsFile * buildFile(std::string & filePath) = 0;

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
    virtual FsFolder * buildFolder(std::string & folderPath, const char * folderName) = 0;

    /**
     * get result of produced hierarchy by buildFolder method
     *
     * @return
     *      root FsComponent entity
     */
    FsComponent * getResult() { return root; }

    /**
     * reset state before making new composition
     */
    void reset() { root = nullptr; }
};