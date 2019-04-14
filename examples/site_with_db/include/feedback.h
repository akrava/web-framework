#pragma once

#include <akrava/web-server/handler.h>

class HandlerFeedback : public Handler {
public:
    HandlerFeedback(const char * ds, HTTP::Method m) : Handler(ds, m) {}
    void exec() override;
};