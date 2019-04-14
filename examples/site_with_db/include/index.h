#pragma once

#include <akrava/web-server/handler.h>

class HandlerIndex : public Handler {
public:
    HandlerIndex(const char * ds, HTTP::Method m) : Handler(ds, m) {}
    void exec() override ;
};
