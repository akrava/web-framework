#pragma once

#include "form_parser.h"

/**
 *
 */
class FormMultipart : public FormParser {
public:
    /**
     *
     */
    FormMultipart();
protected:
    /**
     *
     */
    void actionBeforeSelectingBody() override;

    /**
     *
     */
    void parseNextKey(size_t startPosition, size_t endPosition) override;

    /**
     *
     */
    void parseNextValue(size_t startPosition, size_t endPosition) override;

    /**
     *
     */
    void actionBeforeValueParsing(size_t keyStart, size_t keyEnd) override;
};