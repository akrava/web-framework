#pragma once

#include "form_parser.h"

/**
 *
 */
class FormUrlEncoded : public FormParser {
public:
    /**
     *
     */
    FormUrlEncoded();
protected:
    /**
     *
     */
    void parseNextKey(size_t startPosition, size_t endPosition) override;

    /**
     *
     */
    void parseNextValue(size_t startPosition, size_t endPosition) override;
};
