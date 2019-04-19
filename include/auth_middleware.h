#pragma once

#include <functional>
#include "middleware.h"
#include "entity.h"
#include "auth_strategy.h"

/**
 * @brief This middleware checks authentication
 */
class AuthMiddleware : public Middleware {
    const char * cookieMiddlewareID;
    const char * jsonMiddlewareID;
    Entity * currentUser = nullptr;
    AuthStrategy * strategy = nullptr;
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
        : Middleware(nameID), cookieMiddlewareID(cookieID), jsonMiddlewareID(jsonID) {}

    /**
     * Destructs inner objects
     */
    ~AuthMiddleware() override;

    /**
     * Set serialization of user
     *
     * @param serialize
     *      function that convert user object to string
     */
    void setOnSerialize(std::function<std::string(Entity *)> serialize);

    /**
     * Set deserialization of user
     *
     * @param deserialize
     *      function that get user object from string
     */
    void setOnDeserialize(std::function<Entity *(std::string &)> deserialize);

    /**
     * Set function for authentication
     *
     * @param login
     *      get User object from username and password
     */
    void setOnLogin(std::function<Entity *(std::string &, std::string &)> login);

    /**
     * Check authentication
     *
     * @param userName
     *      user id
     * @param password
     *      user pass
     * @return
     *  true if authenticated successfully
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
     * Perform operation to check users credentials
     */
    void exec() override;

    /**
     * delete all data, saved in internal values, set to default
     */
    void clear() override { Middleware::clear(); currentUser = nullptr; }

    /**
     * Get current authenticated user
     *
     * @return
     *      user object
     */
    Entity * getUser();

    /**
     * Set current authenticated user
     *
     * @param user
     *      user object
     */
    void setUser(Entity * user);

   /**
    * Set template strategy
    *
    * @param strategy
    *       concrete strategy
    */
    void setStrategy(AuthStrategy * strategy);
};