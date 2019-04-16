#pragma once

#include <list>
#include "fs_component.h"

/**
 *
 */
class FsFolder : public FsComponent {
    std::string name;
protected:
    std::list<FsComponent *> children;
public:
    /**
     *
     */
    ~FsFolder() override;

    /**
     *
     * @param name
     * @param fullPath
     */
    FsFolder(std::string & name);

    /**
     *
     * @param name
     * @param fullPath
     */
    FsFolder(const char * name);

    /**
     *
     * @return
     */
    std::string getName() override;

    /**
     *
     * @return
     */
    std::string getFileContent() override;

    /**
     *
     * @param filePath
     * @return
     */
    std::string findFileAndGetContent(std::string & filePath) override;

    /**
     *
     */
    void add(FsComponent * component) override;

    /**
     *
     * @param component
     */
    void remove(FsComponent * component) override;

    /**
     *
     * @return
     */
    bool isComposite() override;
};