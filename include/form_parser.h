#pragma once

#include <unordered_map>
#include "form_entity.h"

typedef std::unordered_map<std::string, std::string> KeyValueStorage;

typedef std::unordered_map<std::string, FormEntity *> KeyEntityStorage;

/**
 *
 */
class FormParser {
protected:
    KeyEntityStorage values;
    FormEntity * valueCurrent;
    std::string keyCurrent;
    std::string formBody;
    std::string valuesDelimiter;
    std::string pairsDelimiter;
public:
    /**
     *
     */
    KeyEntityStorage parseForm(std::string & httpRequest, KeyValueStorage * KVPStorage = nullptr);

    /**
     *
     */
    virtual ~FormParser() = default;
protected:
    /**
     *
     */
    void selectFormBody();

    /**
     *
     */
    virtual void actionBeforeSelectingBody() {};

    /**
     *
     */
    virtual void parseNextKey(size_t startPosition, size_t endPosition) = 0;

    /**
     *
     */
    virtual void parseNextValue(size_t startPosition, size_t endPosition) = 0;

    /**
     *
     */
    virtual void actionBeforeValueParsing(size_t keyStart, size_t keyEnd) {};

    /**
     *
     */
    KeyValueStorage getKeyValuePairs();
};