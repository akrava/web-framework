#pragma once

#include <akrava/web-server/handler.h>

class HandlerCalculatePost : public Handler {
public:
    HandlerCalculatePost(const char * ds, HTTP::Method m) : Handler(ds, m) {}
    void exec() override;
};