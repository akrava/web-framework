#pragma once

#include <response.h>

class DefaultResponse : public Response {
    int statusCode;
public:
    DefaultResponse(int status_code, const char * body = nullptr);
};
