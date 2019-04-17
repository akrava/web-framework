#pragma once

#include "middleware.h"
#include "fs_component.h"
#include "fs_creator.h"

/**
 *
 */
class FsMiddleware : public Middleware {
    FsComponent * folder;
    std::string mountPath;
    std::string value;
    FsCreator creator;
public:
    /**
     *
     */
    FsMiddleware(const char * nameID, const char * folderPath, const char * mountPath);

    /**
     *
     */
    bool autoExec() override;

    /**
     *
     */
    void exec() override;
private:
    /**
     *
     * @return
     */
    static FsBuilder * selectBuilder();
};