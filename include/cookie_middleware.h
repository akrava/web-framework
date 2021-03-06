#pragma once

#include "middleware.h"
#include "cookie_entity.h"
#include "cookie_entity_factory.h"

/**
 * @brief inherited class to parse cookie from http request
 *
 * CookieMiddleware is intended to parse cookie from http request, fill response with cookies
 */
class CookieMiddleware : public Middleware {
    std::unordered_map<std::string, std::unique_ptr<Entity>> responseCookies;
    CookieEntityFactory cookieEntityFactory;
public:
    /**
     * create middleware
     *
     * @param nameID
     *      name id
     */
    explicit CookieMiddleware(const char * nameID);

    /**
     * Check if there are cookie in request
     *
     * @return
     *      true, if there are cookie in request
     */
    bool autoExec() override;

    /**
     * parse cookies from http request
     */
    void exec() override;

    /**
     * add CookieEntity to response cookies
     *
     * @param key
     *      key for entity
     * @param value
     *      CookieEntity object
     */
    void addCookie(const char *key, std::unique_ptr<Entity> value);

    /**
     * set response cookies in response headers
     */
    void insertInResponse();

    /**
     * Get new cookie entity with passed type and value
     *
     * @param entityType
     *      type from EntityType enumeration
     * @param value
     *      serialized value of cookie
     * @return
     *      new CookieEntity object
     */
    std::unique_ptr<Entity> createCookie(CookieEntityFactory::EntityType entityType, std::string & value);

    /**
     * delete all data, saved in internal values, set to default
     */
     void clear() override;
};
