#pragma once

#include "handler.h"

class HandlerLogout : public Handler {
public:
    HandlerLogout(const char * ds, HTTP::Method m) : Handler(ds, m) {}
    void exec() override;
};