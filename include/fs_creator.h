#pragma once

#include "fs_builder.h"

/**
 *
 */
class FsCreator {
    FsBuilder * builder;
public:
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