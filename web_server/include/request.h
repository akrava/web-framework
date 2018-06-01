#pragma once

#include "http.h"
#include "message_body.h"
#include <uri.h>
#include <headers.h>

class Request {
    HTTP::Method method;
    URI * uri;
    HTTP::Version version;
    Headers * headers;
    MessageBody * body;
public:
    Request();
    Request(HTTP::Method method, std::string & URI, HTTP::Version version,
            std::string & headers, std::string & body);
    void setMethod(HTTP::Method method);
    void setURI(URI & uri);
    void setVersion(HTTP::Version version);
    void setHeaders(Headers & headers);
    void setMessageBody(MessageBody & body);
    HTTP::Method getMethod();
    URI * getURI();
    HTTP::Version getVersion();
    Headers * getHeaders();
    MessageBody * getMessageBody();
};
