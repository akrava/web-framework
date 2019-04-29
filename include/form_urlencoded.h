#pragma once

#include "form_parser.h"

/**
 * @brief Concrete template method pattern for parsing form encoded
 *      as application/x-www-form-urlencoded
 *
 * This abstract class is intend to parse application/x-www-form-urlencoded html forms
 */
class FormUrlEncoded : public FormParser {
public:
    /**
     * create concrete template method to parse application/x-www-form-urlencoded html forms
     */
    FormUrlEncoded();
protected:
    /**
     * Parse key of form with application/x-www-form-urlencoded enctype
     *
     * @param startPosition
     *      start position of key in form body
     * @param endPosition
     *      end position of key in form body, exclusive
     */
    void parseNextKey(size_t startPosition, size_t endPosition) override;

    /**
     * Parse value of form with application/x-www-form-urlencoded enctype
     *
     * @param startPosition
     *      start position of value in form body
     * @param endPosition
     *      end position of value in form body, exclusive
     */
    void parseNextValue(size_t startPosition, size_t endPosition) override;
};
