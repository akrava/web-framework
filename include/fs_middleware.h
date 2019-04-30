#pragma once

#include "middleware.h"
#include "fs_component.h"
#include "fs_creator.h"

/**
 * @brief Middleware to host directories with inner files
 *
 * This middleware provide an opportunity to share many files
 *      in directories and subdirectories by passing path to
 *      root directory and mount point of this directory.
 */
class FsMiddleware : public Middleware {
    FsComponent * folder;
    std::string mountPath;
    std::string value;
    FsCreator creator;
public:
    /**
     * create FsMiddleware to serve all files from provided directory
     *      filesystem hierarchy
     *
     * @param nameID
     *      string ID of this middleware
     * @param folderPath
     *      relative or full path to hosted folder
     * @param mountPath
     *      path, that would be added before relative path of files
     *      and all target requests uri should begin with this mount path
     */
    FsMiddleware(const char * nameID, const char * folderPath, const char * mountPath = nullptr);

    /**
     * check, if current request is file in hierarchy of hosted
     *      directory
     * @return
     *      true, if there is a needed file, false otherwise
     */
    bool autoExec() override;

    /**
     * execute current middleware
     */
    void exec() override;
private:
    /**
     * Choose concrete FsBuilder to build filesystem hierarchy
     *      from root directory
     *
     * @return
     *      concrete FsBuilder due to current OS
     */
    static FsBuilder * selectBuilder();
};