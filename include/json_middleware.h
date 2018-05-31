#pragma once

#include <middleware.h>
#include <nlohmann/json.hpp>

class JsonMiddleware : public Middleware {
    nlohmann::json jsonRequest;
    nlohmann::json jsonResponse;
    bool errorDeserialize;
public:
    JsonMiddleware(const char * nameID, Request * request, Response * response);
    bool autoExec();
    void exec();
    void setEchoReply();
    void fillResponse();
};


