#pragma once

#include <akrava/web-server/handler.h>

class HandlerOrderPost : public Handler {
public:
    HandlerOrderPost(const char * ds, HTTP::Method m) : Handler(ds, m) {}
    void exec() override;
};