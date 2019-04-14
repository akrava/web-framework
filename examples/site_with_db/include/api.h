#pragma once

#include <akrava/web-server/handler.h>

class HandlerApi : public Handler {
public:
    HandlerApi(const char * ds, HTTP::Method m) : Handler(ds, m) {}
    void exec() override;
};