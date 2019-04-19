#pragma once

#include <tuple>
#include "headers.h"
#include "response.h"

/**
 *
 */
class AuthStrategy {
public:
    /**
     *
     */
    virtual ~AuthStrategy() = default;

    /**
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