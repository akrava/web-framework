#pragma once

#include <akrava/web-server/handler.h>

class HandlerCalculate : public Handler {
public:
    HandlerCalculate(const char * ds, HTTP::Method m) : Handler(ds, m) {}
    void exec() override;
};