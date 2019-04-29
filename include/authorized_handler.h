#pragma once

#include <functional>
#include "handler.h"
#include "entity.h"

/**
 * @brief Proxy handler template to control access to another handlers
 *
 * Any handlers or chain of handlers could be wrapped by this handler. All
 *      that you need is to create an example of this class and pass a concrete
 *      handler, which should be controlled, and function, which checks if
 *      current user could access such resource. Could substitute passed
 *      handlers with 403 or 401 error pages
 */
class AuthorizedHandler : public Handler {
    const char * authMiddlewareID;
    Handler * handler;
    std::function<bool(Entity *)> checkAccess;
public:
    /**
     * Create proxy handler, which wraps passed one
     *
     * @param realHandler
     *      handler and other handlers connected after it, which should be wrapped
     * @param middlewareID
     *      ID of AuthMiddleware, added to application
     * @param checkAccess
     *      function, that checks, if current user could see such resource. If not,
     *      user should see 403 forbidden error page, if there are no user authenticated -
     *      401 unauthenticated error page
     */
    AuthorizedHandler(
        Handler * realHandler,
        const char * middlewareID,
        std::function<bool(Entity *)> checkAccess = nullptr
    );

    /**
     * destructs inner oblects
     */
    ~AuthorizedHandler() override;

    /**
     * Execute real handler, if user has access
     */
    void exec() override;

    /**
     * Set function, that checks if user could access resource
     *
     * @param checkAccess
     *      function that return true if user has access
     */
    void setAccessCheck(std::function<bool(Entity *)> checkAccess);

    /**
     * Set next handler after wrapped handler
     *
     * @param next
     *      handler object
     */
    void setNext(Handler * next) override;

   /**
    * Add handler to tail of wrapped handler chain of responsibility
    *
    * @param handler
    *       handler object
    */
    void add(Handler * handler) override;
};
