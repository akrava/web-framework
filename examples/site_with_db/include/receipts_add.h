#pragma once

#include <akrava/web-server/handler.h>

class HandlerReceiptsAdd : public Handler {
public:
    HandlerReceiptsAdd(const char * ds, HTTP::Method m) : Handler(ds, m) {}
    void exec() override;
};