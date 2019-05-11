#pragma once

#include <akrava/web-server/handler.h>

class HandlerReceiptsDelete : public Handler {
public:
    HandlerReceiptsDelete(const char * ds, HTTP::Method m) : Handler(ds, m) {}
    void exec() override;
};