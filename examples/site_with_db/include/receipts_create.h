#pragma once

#include <akrava/web-server/handler.h>

class HandlerReceiptsCreate : public Handler {
public:
    HandlerReceiptsCreate(const char * ds, HTTP::Method m) : Handler(ds, m) {}
    void exec() override;
};