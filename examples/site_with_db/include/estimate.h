#pragma once

#include <akrava/web-server/handler.h>

class HandlerEstimate : public Handler {
public:
    HandlerEstimate(const char * ds, HTTP::Method m) : Handler(ds, m) {}
    void exec() override;
};