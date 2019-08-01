#pragma once

#include <unordered_map>
#include "form_entity.h"

typedef std::unordered_map<std::string, std::string> KeyValueStorage;
typedef std::unordered_map<std::string, FormEntity *> KeyEntityStorage;

/**
 * @brief Template methods pattern for parsing form
 *
 * This abstract class is intend to parse html form with different
 *      encodings. Template method is parseForm, which consists of
 *      abstracts methods, that could be implemented. There are
 *      FormMultipartData and FormUrlencoded concrete template
 *      methods in framework
 */
class FormParser {
protected:
    KeyEntityStorage * values = nullptr;
    FormEntity * valueCurrent = nullptr;
    std::string keyCurrent;
    std::string formBody;
    std::string valuesDelimiter;
    std::string pairsDelimiter;
public:
    /**
     * Template method to parse html forms
     *
     * @param httpRequest
     *      serialized http request
     * @param KVPStorage
     *      out parameter. If not null, write key value pairs (string, string)
     *      to out parameter
     * @return
     *      key value pairs (string, FormEntity object)
     */
    KeyEntityStorage * parseForm(std::string & httpRequest, KeyValueStorage * KVPStorage = nullptr);

    /**
     * delete internal values
     */
    virtual ~FormParser() = default;
protected:
    /**
     * Parse http request to find body of form and then store it in parser
     */
    void selectFormBody();

    /**
     * not required method to perform actions before finding body of form.
     *      By default do nothing
     */
    virtual void actionBeforeSelectingBody() {};

    /**
     * Parse key of html form
     *
     * @param startPosition
     *      start position of key in form body
     * @param endPosition
     *      end position of key in form body, exclusive
     */
    virtual void parseNextKey(size_t startPosition, size_t endPosition) = 0;

    /**
     * Parse value of html form
     *
     * @param startPosition
     *      start position of value in form body
     * @param endPosition
     *      end position of value in form body, exclusive
     */
    virtual void parseNextValue(size_t startPosition, size_t endPosition) = 0;

    /**
     * not required method to perform actions before parsing key of form.
     *      By default do nothing
     *
     * @param keyStart
     *      start position of key in form body
     * @param keyEnd
     *      end position of key in form body, exclusive
     */
    virtual void actionBeforeValueParsing(size_t keyStart, size_t keyEnd) {};

    /**
     * generate key value (string, string) pairs from from
     *
     * @return
     *      key value pairs (string, string)
     */
    KeyValueStorage getKeyValuePairs();

    /**
     *
     * @param valueCurrent
     */
    void setCurrentValue(FormEntity * valueCurrent);
};