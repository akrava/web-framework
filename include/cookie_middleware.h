#pragma once

#include <middleware.h>
#include <cookie_entity.h>

class CookieMiddleware : public Middleware {
    std::unordered_map<std::string, CookieEntity> responseCookies;
public:
    CookieMiddleware(const char * nameID, Request * request, Response * response);
    bool autoExec();
    void exec();
    void addCooike(const char * key, CookieEntity & value);
    void insertInResponse();
};
