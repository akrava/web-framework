#pragma once

#include <akrava/web-server/handler.h>

class HandlerEstimatePost : public Handler {
public:
    HandlerEstimatePost(const char * ds, HTTP::Method m) : Handler(ds, m) {}
    void exec() override;
};