#pragma once
#include <http.h>
#include <uri.h>
#include <headers.h>
#include <request.h>
#include <response.h>
#include <message_body.h>
#include "middleware.h"
#include <vector>

class Context {
    Request * request;
    Response * response;
    std::vector<Middleware *> * middlewareList;
public:
    Context();
    Context(Request & request);
    Context(Request & request, Response & response, std::vector<Middleware *> * middlewareList);
    Request * getRequest();
    Response * getResponse();
    void setRequest(Request & request);
    void setResponse(Response & response);
    void setMiddlewareList(std::vector<Middleware *> * middlewareList);
    void setPermanentlyRedirect(const char * uri);
    void setTemporaryRedirect(const char * uri);
    void setRedirect(const char * uri, int code);
    Middleware * getMiddlewareByNameID(const char * nameID);
};