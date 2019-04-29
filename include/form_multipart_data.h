#pragma once

#include "form_parser.h"

/**
 * @brief Concrete template method pattern for parsing form encoded as multipart/from-data
 *
 * This abstract class is intend to parse multipart/from-data html forms
 */
class FormMultipart : public FormParser {
public:
    /**
     * create concrete template method to parse multipart/from-data html forms
     */
    FormMultipart();
protected:
    /**
     * parse delimiters between key value pairs in headers, trim form body
     */
    void actionBeforeSelectingBody() override;

    /**
     * Parse key of form with multipart/from-data enctype
     *
     * @param startPosition
     *      start position of key in form body
     * @param endPosition
     *      end position of key in form body, exclusive
     */
    void parseNextKey(size_t startPosition, size_t endPosition) override;

    /**
     * Parse value of form with multipart/from-data enctype
     *
     * @param startPosition
     *      start position of value in form body
     * @param endPosition
     *      end position of value in form body, exclusive
     */
    void parseNextValue(size_t startPosition, size_t endPosition) override;

    /**
     * parse additional form value properties (if value is file)
     *
     * @param keyStart
     *      start position of key in form body
     * @param keyEnd
     *      end position of key in form body, exclusive
     */
    void actionBeforeValueParsing(size_t keyStart, size_t keyEnd) override;
};