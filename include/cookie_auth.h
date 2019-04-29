#pragma once

#include "auth_strategy.h"
#include "middleware.h"

/**
 * @brief Concrete cookie strategy of AuthStrategy
 *
 * This class is implementation of authentication clients by using cookies,
 *      in which some serialized data of user is stored to recognize him in
 *      following requests. It uses sha5256 algorithm to sign data
 */
class CookieAuth : public AuthStrategy {
    std::string secret;
    Middleware * middleware;
public:
   /**
    * Create Cookie Auth middleware
    *
    * @param secret
    *       secret key used to sign data and prevent outside injections
    * @param cookie
    *       object of cookie middleware to parse cookies
    */
    CookieAuth(const char * secret, Middleware * cookie);

    /**
     * Check, if there are any cookies with data of authenticated user
     *
     * @param headers
     *      headers object
     * @return
     *      true, if this strategy should exec
     */
    bool checkHeadersToParse(Headers * headers) override;

    /**
     * Get user serialized data
     *
     * @param headers
     *      headers object
     * @return
     *      user serialized data
     */
    std::string getSerializedValue(Headers * headers) override;

    /**
     * Sign serialized data and set both to response cookies
     *
     * @param value
     *      serialized data of user
     */
    void serializeAndSetValueToResponse(std::string & value) override;
};