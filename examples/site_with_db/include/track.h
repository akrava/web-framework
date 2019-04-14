#pragma once

#include <akrava/web-server/handler.h>

class HandlerTrack : public Handler {
public:
    HandlerTrack(const char * ds, HTTP::Method m) : Handler(ds, m) {}
    void exec() override;
};