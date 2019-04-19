#pragma once

#include <functional>
#include "middleware.h"
#include "entity.h"
#include "auth_strategy.h"

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
     * create middleware
     *
     * @param nameID
     *      name id
     */
    AuthMiddleware(const char * nameID, const char * cookieID, const char * jsonID)
        : Middleware(nameID), cookieMiddlewareID(cookieID), jsonMiddlewareID(jsonID) {}

    /**
     *
     */
    ~AuthMiddleware() override;

    /**
     *
     */
    void setOnSerialize(std::function<std::string(Entity *)> serialize);

    /**
     *
     */
    void setOnDeserialize(std::function<Entity *(std::string &)> deserialize);

    /**
     *
     */
    void setOnLogin(std::function<Entity *(std::string &, std::string &)> login);

    /**
     *
     */
    bool login(std::string & userName, std::string & password);

    /**
     *
     */
    bool autoExec() override;

    /**
     *
     */
    void exec() override;

    /**
     * delete all data, saved in internal values, set to default
     */
    void clear() override { Middleware::clear(); currentUser = nullptr; }

    /**
     *
     * @return
     */
    Entity * getUser();

    /**
     *
     * @param id
     */
    void setUser(Entity * user);

    /**
     *
     */
    void setStrategy(AuthStrategy * strategy);
};