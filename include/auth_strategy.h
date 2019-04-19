#pragma once

#include <tuple>
#include "headers.h"
#include "response.h"

/**
 * Interface for auth strategy
 */
class AuthStrategy {
public:
    /**
     * Destructor
     */
    virtual ~AuthStrategy() = default;

    /**
     * Parse
     *
     * @param headers
     * @return
     */
    virtual std::string getSerializedValue(Headers * headers) = 0;

    /**
     *
     * @param req
     * @param value
     */
    virtual void serializeAndSetValueToResponse(std::string & value) = 0;
};