#pragma once

#include <akrava/web-server/handler.h>

class HandlerReceiptsEdit : public Handler {
public:
    HandlerReceiptsEdit(const char * ds, HTTP::Method m) : Handler(ds, m) {}
    void exec() override;
};