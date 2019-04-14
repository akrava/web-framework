#pragma once

#include <akrava/web-server/handler.h>

class HandlerFeedbackPost : public Handler {
public:
    HandlerFeedbackPost(const char * ds, HTTP::Method m) : Handler(ds, m) {}
    void exec() override;
};