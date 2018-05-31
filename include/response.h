#pragma once
#include "http.h"
#include "headers.h"
#include "message_body.h"

class Response {
    HTTP::Version version;
    int status;
    Headers * headers;
    MessageBody * body;
public:
    Response();
    Response(HTTP::Version version, int status, Headers & headers,
             MessageBody & body);
    void setVersion(HTTP::Version version);
    void setStatus(int status);
    void setHeaders(Headers & headers);
    void setBody(MessageBody & body);
    HTTP::Version getVersion();
    int getStatus();
    Headers * getHeaders();
    MessageBody * getBody();
};
