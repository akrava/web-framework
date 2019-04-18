#pragma once

#include "middleware.h"
#include "form_parser.h"

/**
 * @brief inherited class to parse application/x-www-form-urlencoded
 *
 * FormMiddleware is intended to parse forms from http request and decode it
 */
class FormMiddleware : public Middleware {
    FormParser * currentParser = nullptr;
public:
    /**
     * create middleware
     *
     * @param nameID
     *      name id
     */
    FormMiddleware(const char * nameID) : Middleware(nameID) {}

    /**
     *
     */
    ~FormMiddleware();

    /**
     * Check if request is application/x-www-form-urlencoded
     *
     * @return
     *      true, if content type of http request is form
     */
    bool autoExec();

    /**
     * parse form in http request
     */
    void exec();

    /**
     * delete all data, saved in internal values, set to default
     */
    void clear() { Middleware::clear(); }

    /**
     *
     * @param parser
     */
    void setParser(FormParser * parser);

    /**
     *
     * @return
     */
    FormParser * getParser();
};
