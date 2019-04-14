#pragma once

#include <akrava/web-server/handler.h>

class HandlerTemplate : public Handler {
public:
    HandlerTemplate() : Handler() {}
    void exec() override;
};