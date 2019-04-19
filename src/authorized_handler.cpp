#include "authorized_handler.h"
#include "auth_middleware.h"
#include "runtime_exception.h"
#include "default_response.h"

AuthorizedHandler::AuthorizedHandler(
        Handler *realHandler, const char* id, std::function<bool(Entity *)> checkAccess
    ) : Handler(realHandler->getRoute().c_str(), realHandler->getMethod()) {
        authMiddlewareID = id;
        handler = realHandler;
        if (checkAccess) {
            this->checkAccess = checkAccess;
        }
}

AuthorizedHandler::~AuthorizedHandler() {
    delete handler;
}

void AuthorizedHandler::exec() {
    if (!checkAccess) {
        throw RuntimeException("Check access function is not set");
    }
    auto authMiddleware = (AuthMiddleware *)getContext()->getMiddlewareByNameID(authMiddlewareID);
    if (!authMiddleware) {
        throw RuntimeException("Auth middleware could nor be found");
    }
    auto user = authMiddleware->getUser();
    if (!user) {
        auto authTroubleResponse = new DefaultResponse{401, "Unauthorized"};
        authTroubleResponse->finalize();
        getContext()->setResponse(authTroubleResponse);
        return;
    }
    if (checkAccess(user)) {
        handler->setContext(getContext());
        handler->exec();
    } else {
        auto accessTroubleResponse = new DefaultResponse{403, "Forbidden "};
        accessTroubleResponse->finalize();
        getContext()->setResponse(accessTroubleResponse);
    }
}

void AuthorizedHandler::setAccessCheck(std::function<bool(Entity *)> checkAccess) {
    this->checkAccess = checkAccess;
}
