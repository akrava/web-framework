#pragma once

#include "fs_builder.h"

/**
 * @brief Director for builder template for fs middleware
 */
class FsCreator {
    FsBuilder * builder = nullptr;
public:
    /**
     *
     */
    ~FsCreator();

    /**
     *
     * @param builder
     */
    void setBuilder(FsBuilder * builder);

    /**
     *
     * @param folderPath
     */
    void makeFs(std::string & folderPath);

    /**
     *
     * @param builder
     * @param folderPath
     */
    void makeFs(FsBuilder * builder, std::string & folderPath);

    /**
     *
     * @return
     */
    FsComponent * getFs();
};