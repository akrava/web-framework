#include "logout.h"
#include <akrava/web-server/file_handler.h>
#include <akrava/web-server/cookie_middleware.h>
#include "login.h"
#include "config.h"

using namespace std;

void HandlerLogout::exec() {
    Middleware * middleware = getContext()->getMiddlewareByNameID("cookie");
    if (!middleware) return;
    auto * cookie = (CookieMiddleware *) middleware;
    auto cookieType = CookieEntityFactory::EntityType::HttpOnly;
    string empty;
    cookie->addCookie("X-Access-Token", cookie->createCookie(cookieType, empty));
    cookie->insertInResponse();
    auto res = getContext()->getResponse();
    res->setStatus(302);
    res->setVersion(HTTP::Version::HTTP_1_1);
    res->getHeaders()->add("Location", "/");
}
