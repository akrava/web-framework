#pragma once

#include <akrava/web-server/handler.h>

class HandlerReceipts : public Handler {
public:
    HandlerReceipts(const char * ds, HTTP::Method m) : Handler(ds, m) {}
    void exec() override;
};