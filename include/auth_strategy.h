#pragma once

#include "headers.h"
#include "response.h"

/**
 * @brief Interface for auth strategy template
 *
 * This is abstract class for authentication strategies. You can create
 *      your own strategy, which inherits AuthStrategy class and implements
 *      all methods below. CookieAuth and JwtAuth are defined in this framework.
 */
class AuthStrategy {
public:
    /**
     * virtual destructor
     */
    virtual ~AuthStrategy() = default;

    /**
     * Parse headers to get serialized user credentials
     *
     * @param headers
     *      current request headers
     * @return
     *      serialized user in string
     */
    virtual std::string getSerializedValue(Headers * headers) = 0;

    /**
     * Check, if this strategy could be executed with such request headers
     *
     * @param headers
     *      current request headers
     * @return
     *      true, if there are some data in headers, which this strategy
     *      could proceed, false otherwise
     */
    virtual bool checkHeadersToParse(Headers * headers) = 0;

    /**
     * Set to current response user credentials
     *
     * @param value
     *      serialized user
     */
    virtual void serializeAndSetValueToResponse(std::string & value) = 0;
};