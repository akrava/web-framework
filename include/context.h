#pragma once
#include <http.h>
#include <uri.h>
#include <headers.h>
#include <request.h>
#include <response.h>
#include <message_body.h>

class Context {
    Request request;
    Response response;
public:
    Context();
    Context(Request & request);
    Context(Request & request, Response & response);
    Request getRequest();
    Response getResponse();
    void setRequest(Request & request);
    void setResponse(Response & response);
    void setPermanentlyRedirect(const char * uri);
    void setTemporaryRedirect(const char * uri);
    void setRedirect(const char * uri, int code);
};
