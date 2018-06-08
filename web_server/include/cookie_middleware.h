#pragma once

#include <middleware.h>
#include <cookie_entity.h>

/**
 *
 */
class CookieMiddleware : public Middleware {
    std::unordered_map<std::string, CookieEntity> responseCookies;
public:
    /**
     *
     * @param nameID
     * @param request
     * @param response
     */
    CookieMiddleware(const char * nameID, Request * request, Response * response);

    /**
     *
     * @return
     */
    bool autoExec();

    /**
     *
     */
    void exec();

    /**
     *
     * @param key
     * @param value
     */
    void addCookie(const char *key, CookieEntity &value);

    /**
     *
     */
    void insertInResponse();
};
