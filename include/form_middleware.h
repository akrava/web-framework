#pragma once

#include <middleware.h>

class FormMiddleware : public Middleware {

public:
    FormMiddleware(const char * nameID, Request * request, Response * response)
            : Middleware(nameID, request, response){}
    bool autoExec();
    void exec();
};


