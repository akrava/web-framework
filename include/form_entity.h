#pragma once

#include <string>
#include <list>

/**
 * @brief Wrapper class for value form
 *
 * Object of this class could store data and some additional
 *      information about parsed value
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
     * Create parsed form value entity
     *
     * @param value
     *      serialized value
     */
    explicit FormEntity(std::string & value);

    /**
     * delete all inner objects
     */
    ~FormEntity();

    /**
     * Set additional file name property to form value
     *
     * @param filename
     *      name of file, loaded via form
     */
    void setFileName(std::string & filename);

    /**
     * Get additional file name property to form value
     *
     * @return
     *      name of file, loaded via form
     */
    std::string getFileName();

    /**
     * Set additional content type property to form value
     *
     * @param contentType
     *      content type of loaded file
     */
    void setContentType(std::string & contentType);

    /**
     * Get additional content type property to form value
     *
     * @return
     *      content type of loaded file
     */
    std::string getContentType();

    /**
     * Set charset property to form value
     *
     * @param charset
     *      encoding of form value
     */
    void setCharset(std::string charset);

    /**
     * Get charset property to form value
     *
     * @return
     *      encoding of form value
     */
    std::string getCharset();

    /**
     * Set serialized value of form entity
     *
     * @return
     *      serialized value passed by from
     */
    void setValue(std::string & val);

    /**
     * Get serialized value of form entity
     *
     * @return
     *      serialized value passed by from
     */
    std::string getValue();

    /**
     * Check, if this form value is file
     *
     * @return
     *      true, if this object is file, false - otherwise
     */
    bool isFileContent();

    /**
     * Add another form value with same name
     *
     * @param next
     *      FormEntity object
     */
    void setNext(FormEntity * next);

    /**
     * Get another form value with same name
     *
     * @return
     *      FormEntity object
     */
    FormEntity * getNext();

    /**
     * Get all form values with same name
     *
     * @return
     *      list of FormEntity objects
     */
    std::list<FormEntity *> getArrayValue();
};