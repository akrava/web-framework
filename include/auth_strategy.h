#pragma once

#include <tuple>
#include "headers.h"
#include "response.h"

/**
 * @brief Interface for auth strategy
 */
class AuthStrategy {
public:
    /**
     * Destructor
     */
    virtual ~AuthStrategy() = default;

    /**
     * Parse headers to get serialized user credentials
     *
     * @param headers
     *      current request headers
     * @return
     *      serialized user
     */
    virtual std::string getSerializedValue(Headers * headers) = 0;

    /**
     * Set to response user credentials
     *
     * @param value
     *      serialized user
     */
    virtual void serializeAndSetValueToResponse(std::string & value) = 0;
};