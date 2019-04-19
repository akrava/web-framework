#pragma once

#include <akrava/web-server/handler.h>

class AuthHandler : public Handler {
public:
    AuthHandler(const char * ds, HTTP::Method m) : Handler(ds, m) {}
    void exec() override;
};
