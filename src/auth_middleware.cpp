#include <runtime_exception.h>
#include "auth_middleware.h"
#include "cookie_auth.h"
#include "jwt_auth.h"

Entity * AuthMiddleware::getUser() {
    if (currentUser) {
        return currentUser;
    }
    return nullptr;
}

void AuthMiddleware::setUser(Entity * user) {
    delete currentUser;
    currentUser = user;
}

void AuthMiddleware::setOnSerialize(std::function<std::string(Entity *)> serialize) {
    this->onSerialize = serialize;
}

void AuthMiddleware::setOnDeserialize(std::function<Entity *(std::string &)> deserialize) {
    this->onDeserialize = deserialize;
}

void AuthMiddleware::setOnLogin(std::function<Entity *(std::string &, std::string &)> login) {
    this->onLogin = login;
}

bool AuthMiddleware::autoExec() {
    auto headers = request->getHeaders();
    return CookieAuth::checkHeadersToParse(headers) || JwtAuth::checkHeadersToParse(headers);
}

void AuthMiddleware::exec() {
    if (!strategy) {
        throw RuntimeException("Strategy was not set");
    }
    auto value = strategy->getSerializedValue(request->getHeaders());
    if (value.empty()) {
        return;
    }
    auto user = onDeserialize(value);
    setUser(user);
}

void AuthMiddleware::setStrategy(AuthStrategy * strategy) {
    delete this->strategy;
    this->strategy = strategy;
}

AuthMiddleware::~AuthMiddleware() {
    delete currentUser;
    delete strategy;
}

bool AuthMiddleware::login(std::string & userName, std::string & password) {
    auto userObject = onLogin(userName, password);
    if (userObject == nullptr) {
        return false;
    } else {
        setUser(userObject);
        std::string serializedUser = onSerialize(userObject);
        strategy->serializeAndSetValueToResponse(serializedUser);
        return true;
    }
}
