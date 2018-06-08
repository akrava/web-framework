#pragma once

#include "http.h"
#include "headers.h"
#include "message_body.h"

/**
 *
 */
class Response {
    HTTP::Version version;
    int status;
    Headers * headers;
    MessageBody * body;
public:
    /**
     *
     */
    Response();

    /**
     *
     * @param version
     * @param status
     * @param headers
     * @param body
     */
    Response(HTTP::Version version, int status, Headers & headers,
             MessageBody & body);

    ~Response();

    /**
     *
     * @param version
     */
    void setVersion(HTTP::Version version);

    /**
     *
     * @param status
     */
    void setStatus(int status);

    /**
     *
     * @param headers
     */
    void setHeaders(Headers & headers);

    /**
     *
     * @param body
     */
    void setBody(MessageBody & body);

    /**
     *
     * @return
     */
    HTTP::Version getVersion();

    /**
     *
     * @return
     */
    int getStatus();

    /**
     *
     * @return
     */
    Headers * getHeaders();

    /**
     *
     * @return
     */
    MessageBody * getBody();
};
