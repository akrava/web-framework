#include "cookie_auth.h"
#include "runtime_exception.h"
#include "parser_http.h"
#include "cookie_middleware.h"

using namespace std;

CookieAuth::CookieAuth(const char * secret, Middleware * cookie) {
    this->secret = secret;
    middleware = cookie;
}

bool CookieAuth::checkHeadersToParse(Headers *headers) {
    if (!headers) {
        throw RuntimeException("Headers was not set");
    }
    std::string value;
    if (!headers->getValue("Cookie", value) || value.empty()) {
        return false;
    }
    return value.find("X-Access-Token") != string::npos;
}

std::string CookieAuth::getSerializedValue(Headers *headers) {
    std::string value;
    std::string sign;
    if (!middleware)  {
        throw RuntimeException("Middleware was not set");
    }
    auto cookie = (CookieMiddleware *) middleware;
    bool result = cookie->getValueFromMap("X-Access-Token", value);
    result = result && cookie->getValueFromMap("X-Access-Sign", sign);
    if (!result || ParserHTTP::sha256(secret, value) != sign) {
        return "";
    }
    return value;
}

void CookieAuth::serializeAndSetValueToResponse(std::string & value) {
    std::string sign = ParserHTTP::sha256(secret, value);
    if (!middleware)  {
        throw RuntimeException("Middleware was not set");
    }
    auto cookie = (CookieMiddleware *) middleware;
    auto cookieType = CookieEntityFactory::EntityType::HttpOnly;
    cookie->addCookie("X-Access-Token", cookie->createCookie(cookieType, value));
    cookie->addCookie("X-Access-Sign", cookie->createCookie(cookieType, sign));
    cookie->insertInResponse();
}
