#pragma once

#include <functional>
#include "handler.h"
#include "entity.h"

/**
 *
 */
class AuthorizedHandler : public Handler {
    const char * authMiddlewareID;
    Handler * handler;
    std::function<bool(Entity *)> checkAccess;
public:
    /**
     *
     */
    AuthorizedHandler(
        Handler * realHandler,
        const char * middlewareID,
        std::function<bool(Entity *)> checkAccess = nullptr
    );

    /**
     *
     */
    ~AuthorizedHandler() override;

    /**
     *
     */
    void exec() override;

    /**
     *
     */
    void setAccessCheck(std::function<bool(Entity *)> checkAccess);
};
