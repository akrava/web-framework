#pragma once

#include <string>

/**
 *
 */
class FsComponent {
public:
    /**
     *
     */
    virtual ~FsComponent() = default;

    /**
     *
     * @return
     */
    virtual std::string getName() = 0;

    /**
     *
     * @return
     */
    virtual std::string getFileContent() = 0;

    /**
     *
     * @param filePath
     * @return
     */
    virtual std::string findFileAndGetContent(std::string & filePath) = 0;

    /**
     *
     */
    virtual void add(FsComponent * component) = 0;

    /**
     *
     * @param component
     */
    virtual void remove(FsComponent * component) = 0;

    /**
     *
     * @return
     */
    virtual bool isComposite() = 0;
};