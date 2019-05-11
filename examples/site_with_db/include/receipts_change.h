#pragma once

#include <akrava/web-server/handler.h>

class HandlerReceiptsChange : public Handler {
public:
    HandlerReceiptsChange(const char * ds, HTTP::Method m) : Handler(ds, m) {}
    void exec() override;
};