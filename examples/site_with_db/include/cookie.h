#pragma once

#include <akrava/web-server/handler.h>

class HandlerCookie : public Handler {
public:
    HandlerCookie() : Handler() {}
    void exec() override;
};