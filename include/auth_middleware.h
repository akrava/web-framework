#pragma once

#include "middleware.h"

class AuthMiddleware : public Middleware {
public:
    /**
     * create middleware
     *
     * @param nameID
     *      name id
     */
    AuthMiddleware(const char * nameID) : Middleware(nameID) {}

    /**
     *
     */
    bool autoExec() { return false; };

    /**
     *
     */
    void exec() {};

    /**
     * delete all data, saved in internal values, set to default
     */
    void clear() { Middleware::clear(); }

    /**
     *
     * @return
     */
    std::string getUserId();

    /**
     *
     * @param id
     */
    void setUser(std::string & id);
};