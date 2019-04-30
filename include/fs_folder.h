#pragma once

#include <list>
#include "fs_component.h"

/**
 * @brief Representation of folder in filesystem
 *
 * This class is intended to represent a folder in filesystem hierarchy
 */
class FsFolder : public FsComponent {
    std::string name;
protected:
    std::list<FsComponent *> children;
public:
    /**
     * delete all inner hierarchy
     */
    ~FsFolder() override;

    /**
     * Create a FsFolder object with specified name
     *
     * @param name
     *      name of current folder as string
     */
    explicit FsFolder(std::string & name);

    /**
     * Create a FsFolder object with specified name
     *
     * @param name
     *      name of current folder as const char *
     */
    explicit FsFolder(const char * name);

    /**
     * get name of current folder
     *
     * @return
     *      name of folder as string
     */
    std::string getName() override;

    /**
     * @throw
     *      RuntimeException, if this component was not
     *      a file
     */
    std::string getFileContent() override;

    /**
     * recursively find a file with such path in folder
     *      hierarchy and read it's content
     *
     * @param filePath
     *      file path of file relatively root folder
     * @return
     *      content, that was read from file with specified filepath
     */
    std::string findFileAndGetContent(std::string & filePath) override;

    /**
     * Add FsComponent to current entity's children
     *
     * @param component
     *      FsComponent object - file or folder
     */
    void add(FsComponent * component) override;

    /**
     * Remove FsComponent from current entity's children
     *
     * @param component
     *      FsComponent object - file or folder
     */
    void remove(FsComponent * component) override;

    /**
     * Check, if current entity is composite and have children
     *
     * @return
     *      true, because it's a folder
     */
    bool isComposite() override;
};