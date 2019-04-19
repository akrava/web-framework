#pragma once

#include <string>
#include <list>

/**
 * @brief Wrapper for form value
 */
class FormEntity {
    std::string value;
    std::string fileName;
    std::string contentType;
    std::string charset;
    FormEntity * next;
    bool isFile;
public:
    /**
     *
     * @param value
     */
    FormEntity(std::string & value);

    /**
     *
     */
    ~FormEntity();

    /**
     *
     * @param filename
     */
    void setFileName(std::string & filename);

    /**
     *
     * @return
     */
    std::string getFileName();

    /**
     *
     * @param contentType
     */
    void setContentType(std::string & contentType);

    /**
     *
     * @return
     */
    std::string getContentType();

    /**
     *
     * @param charset
     */
    void setCharset(std::string charset);

    /**
     *
     * @return
     */
    std::string getCharset();

    /**
     *
     * @return
     */
    void setValue(std::string & val);

    /**
     *
     * @return
     */
    std::string getValue();

    /**
     *
     * @return
     */
    bool isFileContent();

    /**
     *
     * @param next
     */
    void setNext(FormEntity * next);

    /**
     *
     * @return
     */
    FormEntity * getNext();

    /**
     *
     * @return
     */
    std::list<FormEntity *> getArrayValue();
};