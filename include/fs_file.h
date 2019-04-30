#pragma once

#include "fs_component.h"

/**
 * @brief Representation of file in filesystem
 *
 * This class is intended to represent a file in filesystem hierarchy
 */
class FsFile : public FsComponent {
    std::string fullPath;
    std::string name;
public:
    /**
     * Create a FsFile object with specified name and full path
     *
     * @param name
     *      file name
     * @param fullPath
     *      full path to file
     */
    FsFile(std::string & name, std::string & fullPath);

    /**
     * get name of current file
     *
     * @return
     *      name of file as string
     */
    std::string getName() override;

    /**
     * returns a content, stored in this file.
     *
     * @return
     *      content, that was read from file
     */
    std::string getFileContent() override;

    /**
     * @throw
     *      RuntimeException, because it's not a folder
     */
    std::string findFileAndGetContent(std::string & filePath) override;

    /**
     * @throw
     *      RuntimeException, because it's not a folder
     */
    void add(FsComponent * component) override;

    /**
     * @throw
     *      RuntimeException, because it's not a folder
     */
    void remove(FsComponent * component) override;

    /**
     * Check, if current entity is composite and have children
     *
     * @return
     *      false, because it's not a folder
     */
    bool isComposite() override;
};