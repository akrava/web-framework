#pragma once

#include <akrava/web-server/handler.h>

class HandlerRenderTemplate : public Handler {
public:
    HandlerRenderTemplate() : Handler() {}
    void exec() override;
};