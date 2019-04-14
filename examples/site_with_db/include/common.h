#pragma once

#include <akrava/web-server/handler.h>

class HandlerCommon : public Handler {
public:
    HandlerCommon() : Handler() {}
    void exec() override;
};