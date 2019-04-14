#pragma once

#include <akrava/web-server/handler.h>

class HandlerMap : public Handler {
public:
    HandlerMap(const char * ds, HTTP::Method m) : Handler(ds, m) {}
    void exec() override;
};