#pragma once

#include "fs_builder.h"

/**
 * @brief Director template for builder template of FsMiddleware
 *
 * This class makes a filesystem hierarchy, delegating this work
 *      to concrete builders, which could be specified
 */
class FsCreator {
    FsBuilder * builder = nullptr;
public:
    /**
     * delete inner objects
     */
    ~FsCreator();

    /**
     * set concrete builder
     *
     * @param builder
     *      FsBuilder object
     */
    void setBuilder(FsBuilder * builder);

    /**
     * build filesystem hierarchy, passing a folder path
     *
     * @param folderPath
     *      build filesystem hierarchy, passing a folder path
     */
    void makeFs(std::string & folderPath);

    /**
     * build filesystem hierarchy, passing a folder path
     *
     * @param builder
     *      set concrete builder
     * @param folderPath
     *      build filesystem hierarchy, passing a folder path
     */
    void makeFs(FsBuilder * builder, std::string & folderPath);

    /**
     * after making filesystem by makeFs method, you can get
     *      produced hierarchy
     *
     * @return
     *      root FsComponent of fs hierarchy
     */
    FsComponent * getFs();
};