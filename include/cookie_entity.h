#pragma once

#include <string>
#include <ctime>
#include <entity.h>

/**
 * @brief Class wrapper for Cookies. Allow you adjust parameters od each http cookie.
 *      Used by CookieMiddleware
 *
 * Object of this class consist of key-value pair, and some options for it, like
 *       date expires, max age, domain, path, option http only
 */
class CookieEntity : public Entity {
    std::string value;
    std::time_t expires;
    size_t maxAge_sec;
    std::string domain;
    std::string path;
    bool httpOnly;
public:
    /**
     * Constructs a cookie entity with parameters
     *
     * @param value
     *      value of cookie
     * @param expires
     *      the maximum lifetime of the cookie as time_t
     * @param maxAge_sec
     *      number of seconds until the cookie expires.
     * @param domain
     *      specifies those hosts to which the cookie will be sent.
     * @param path
     *      indicates a URL path that must exist in the requested resource before
     *      sending the Cookie header
     * @param httpOnly
     *      HTTP-only cookies aren't accessible via JavaScript
     */
    CookieEntity(const char * value, time_t expires = -1, size_t maxAge_sec = std::string::npos,
                 const char * domain = nullptr, const char *  path = nullptr, bool httpOnly = false);

    /**
     * Method is used to serialize itself
     *
     * @return
     *      serialized string like: "<cookie-value>; Expires=<date>; Max-Age=<non-zero-digit> ..."
     */
    std::string toString() override;

    /**
     *
     */
    std::unique_ptr<Entity> clone() override;
};
