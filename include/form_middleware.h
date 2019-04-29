#pragma once

#include "middleware.h"
#include "form_parser.h"

/**
 * @brief inherited class to parse application/x-www-form-urlencoded
 *
 * FormMiddleware is intended to parse forms from http request and decode it
 */
class FormMiddleware : public Middleware {
    KeyEntityStorage * keysStorage = nullptr;
    FormParser * currentParser = nullptr;
public:
    /**
     * create middleware
     *
     * @param nameID
     *      name id
     */
    explicit FormMiddleware(const char * nameID) : Middleware(nameID) {}

    /**
     * delete internal objects
     */
    ~FormMiddleware() override;

    /**
     * Check if request is application/x-www-form-urlencoded
     *
     * @return
     *      true, if content type of http request is form
     */
    bool autoExec() override;

    /**
     * parse form in http request
     */
    void exec() override;

    /**
     * delete all data, saved in internal values, set to default
     */
    void clear() override { Middleware::clear(); }

    /**
     * get entity value form by key
     *
     * @param key
     *      name of value form
     * @return
     *      FormEntity object
     */
    FormEntity * getFormEntity(const char * key);
private:
    /**
     * set template method of FormParser
     *
     * @param parser
     *      object of template FormParser method
     */
    void setParser(FormParser * parser);

    /**
     * get template method of FormParser
     *
     * @return
     *      object of template FormParser method
     */
    FormParser * getParser();
};
