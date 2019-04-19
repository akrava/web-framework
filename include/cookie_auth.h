#pragma once

#include "auth_strategy.h"
#include "middleware.h"

/**
 *
 */
class CookieAuth : public AuthStrategy {
    std::string secret;
    Middleware * middleware;
public:
    /**
     *
     */
    CookieAuth(const char * secret, Middleware * cookie);

    /**
     *
     * @param headers
     * @return
     */
    static bool checkHeadersToParse(Headers * headers);

    /**
     *
     * @param headers
     * @return
     */
    std::string getSerializedValue(Headers * headers) override;

    /**
     *
     * @param req
     * @param value
     */
    void serializeAndSetValueToResponse(std::string & value) override;
};