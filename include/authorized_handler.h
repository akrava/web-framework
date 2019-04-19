#pragma once

#include <functional>
#include "handler.h"
#include "entity.h"

/**
 * @brief Proxy handler to control access to another resources
 */
class AuthorizedHandler : public Handler {
    const char * authMiddlewareID;
    Handler * handler;
    std::function<bool(Entity *)> checkAccess;
public:
    /**
     * Create proxy
     * @param realHandler
     * @param middlewareID
     * @param checkAccess
     */
    AuthorizedHandler(
        Handler * realHandler,
        const char * middlewareID,
        std::function<bool(Entity *)> checkAccess = nullptr
    );

    /**
     * Destructor
     */
    ~AuthorizedHandler() override;

    /**
     * Execute real handler, if user has access
     */
    void exec() override;

    /**
     * Set function, that checks if user could access
     *
     * @param checkAccess
     *      return true if user has access
     */
    void setAccessCheck(std::function<bool(Entity *)> checkAccess);
};
