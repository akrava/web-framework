#pragma once

#include <middleware.h>

class FormMiddleware : public Middleware {
public:
    /**
     *
     * @param nameID
     * @param request
     * @param response
     */
    FormMiddleware(const char * nameID, Request * request, Response * response)
            : Middleware(nameID, request, response) {}

    /**
     *
     * @return
     */
    bool autoExec();

    /**
     *
     */
    void exec();
};
