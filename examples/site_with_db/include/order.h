#pragma once

#include <akrava/web-server/handler.h>

class HandlerOrder : public Handler {
public:
    HandlerOrder(const char * ds, HTTP::Method m) : Handler(ds, m) {}
    void exec() override;
};