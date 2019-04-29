#pragma once

#include <functional>
#include "middleware.h"
#include "entity.h"
#include "auth_strategy.h"

/**
 * @brief This middleware performs authentication on your application
 *
 * AuthMiddleware is intended to parse cookies or other data in headers
 *      with CookieMiddleware or JsonMiddleware, and then get information
 *      about authenticated user, also it could generate special technical
 *      data for responses to identificate clients
 */
class AuthMiddleware : public Middleware {
    const char * cookieMiddlewareID;
    const char * jsonMiddlewareID;
    Entity * currentUser = nullptr;
    AuthStrategy * strategy = nullptr;
    std::vector<AuthStrategy *> allStrategies;
    std::function<Entity *(std::string &, std::string &)> onLogin = nullptr;
    std::function<std::string(Entity *)> onSerialize = nullptr;
    std::function<Entity *(std::string &)> onDeserialize = nullptr;
public:
    /**
     * Create new auth middleware. Depends on cookie and json middleware
     *
     * @param nameID
     *      id of itself
     * @param cookieID
     *      string id of cookie middleware
     * @param jsonID
     *      string id of json middleware
     */
    AuthMiddleware(const char * nameID, const char * cookieID, const char * jsonID)
        : Middleware(nameID), cookieMiddlewareID(cookieID), jsonMiddlewareID(jsonID), allStrategies() {}

    /**
     * Destructs inner objects
     */
    ~AuthMiddleware() override;

    /**
     * Set function, that perform serialization of user object.
     *      This data will be available on each call of deserialization
     *      function
     *
     * @param serialize
     *      function that convert user object to string
     */
    void setOnSerialize(std::function<std::string(Entity *)> serialize);

    /**
     * Set deserialization function of user. From passed string this function
     *      should get user object from string if everything all right or null
     *      otherwise
     *
     * @param deserialize
     *      function that get user object from string
     */
    void setOnDeserialize(std::function<Entity *(std::string &)> deserialize);

    /**
     * Set function for authentication
     *
     * @param login
     *      get User object from username and password. If something is wrong return
     *      null
     */
    void setOnLogin(std::function<Entity *(std::string &, std::string &)> login);

    /**
     * Check authentication. Could be used in any client defined handler, where is
     *      processing user login to application
     *
     * @param userName
     *      user id
     * @param password
     *      user pass
     * @return
     *      true if authenticated successfully
     */
    bool login(std::string & userName, std::string & password);

    /**
     * If found users credentials, perform exec
     *
     * @return
     *      true if should execute
     */
    bool autoExec() override;

    /**
     * Perform operations to check users credentials and proceed them
     */
    void exec() override;

    /**
     * delete all data, saved in internal values, set to default
     */
    void clear() override { Middleware::clear(); currentUser = nullptr; }

    /**
     * Get current authenticated user. Could be used by any client defined handlers.
     *
     * @return
     *      user object, or null if anyone was authorised
     */
    Entity * getUser();

    /**
     * Set template strategy.
     *
     * @param strategy
     *       concrete strategy
     */
    void addStrategy(AuthStrategy * strategy);
private:
    /**
     * Set current authenticated user
     *
     * @param user
     *      user object
     */
    void setUser(Entity * user);

    /**
    * Set template strategy.
    *
    * @param strategy
    *       concrete strategy
    */
    void setStrategy(AuthStrategy * strategy);
};