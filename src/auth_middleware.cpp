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
    for (auto & cur : allStrategies) {
        if (cur->checkHeadersToParse(headers)) {
            setStrategy(cur);
            return true;
        }
    }
    return false;
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
    this->strategy = strategy;
}

AuthMiddleware::~AuthMiddleware() {
    delete currentUser;
    delete strategy;
    for (auto & cur : allStrategies) {
        delete cur;
    }
}

bool AuthMiddleware::login(std::string & userName, std::string & password) {
    auto userObject = onLogin(userName, password);
    if (userObject == nullptr) {
        return false;
    } else {
        setUser(userObject);
        std::string serializedUser = onSerialize(userObject);
		allStrategies[0]->serializeAndSetValueToResponse(serializedUser);
        return true;
    }
}

void AuthMiddleware::addStrategy(AuthStrategy *strategy) {
    if (strategy) {
        allStrategies.push_back(strategy);
    }
}
