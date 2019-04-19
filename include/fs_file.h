#pragma once

#include "fs_component.h"

/**
 * @brief Wrapper for fs file
 */
class FsFile : public FsComponent {
    std::string fullPath;
    std::string name;
public:
    /**
     *
     * @param name
     * @param fullPath
     */
    FsFile(std::string & name, std::string & fullPath);

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