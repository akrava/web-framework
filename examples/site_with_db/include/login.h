#pragma once

#include <akrava/web-server/handler.h>

class HandlerLogin : public Handler {
public:
    HandlerLogin(const char * ds, HTTP::Method m) : Handler(ds, m) {}
    void exec() override;
};
