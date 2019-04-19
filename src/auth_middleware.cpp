#include "auth_middleware.h"

std::string AuthMiddleware::getUserId() {
    std::string id;
    if (getValueFromMap("UserID", id)) {
        return id;
    } else {
        return nullptr;
    }
}

void AuthMiddleware::setUser(std::string & id) {
    addValueToMap("UserID", id.c_str());
}
