#pragma once

#include <akrava/web-server/handler.h>

class HandlerNews : public Handler {
public:
    HandlerNews(const char * ds, HTTP::Method m) : Handler(ds, m) {}
    void exec() override;
};