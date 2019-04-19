#pragma once

#include "auth_strategy.h"
#include "middleware.h"

/**
 * @brief JWT auth strategy
 */
class  JwtAuth : public AuthStrategy {
    std::string secret;
    Middleware * middleware;
    std::string headersToken;
public:
    /**
     *
     */
    JwtAuth(const char * secret, Middleware * json);

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