#pragma once

#include "response.h"

/**
 * @brief Response class which is intended to make sample html pages on status codes
 *
 * Inherited class DefaultResponse from Response for setting stubs for non-realized functionality
 */
class DefaultResponse : public Response {
    int statusCode;
public:
    /**
     * Create DefaultResponse with code status or custom body
     *
     * @param status_code
     *      http code status, set the body for its reason phrase,
     *      if status_code < 0, set the body from param body
     * @param body
     *      custom body page
     */
    DefaultResponse(int status_code, const char * body = nullptr);
};
