#pragma once

#include <string>

/**
 * @brief Interface for filesystem entity - file or path. Composite template
 *
 * This class is intended to represent a filesystem hierarchy as composite
 *      template and quickly find any file with specified path and get it's
 *      content.
 */
class FsComponent {
public:
    /**
     * virtual destructor
     */
    virtual ~FsComponent() = default;

    /**
     * get name of current component
     *
     * @return
     *      name of component as string
     */
    virtual std::string getName() = 0;

    /**
     * returns a content, stored in this file.
     *
     * @return
     *      content, that was read from file
     * @throw
     *      RuntimeException, if this component was not
     *      a file
     */
    virtual std::string getFileContent() = 0;

    /**
     * recursively find a file with such path in folder
     *      hierarchy and read it's content
     *
     * @param filePath
     *      file path of file relatively root folder
     * @return
     *      content, that was read from file with specified filepath
     * @throw
     *      RuntimeException, if this component was not
     *      a folder
     */
    virtual std::string findFileAndGetContent(std::string & filePath) = 0;

    /**
     * Add FsComponent to current entity's children
     *
     * @param component
     *      FsComponent object - file or folder
     * @throw
     *      RuntimeException, if this component was not
     *      a folder
     */
    virtual void add(FsComponent * component) = 0;

    /**
     * Remove FsComponent from current entity's children
     *
     * @param component
     *      FsComponent object - file or folder
     * @throw
     *      RuntimeException, if this component was not
     *      a folder
     */
    virtual void remove(FsComponent * component) = 0;

    /**
     * Check, if current entity is composite and have children
     *
     * @return
     *      true, if this component is folder, false otherwise
     */
    virtual bool isComposite() = 0;
};