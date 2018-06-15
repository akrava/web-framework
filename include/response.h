#pragma once

#include "http.h"
#include "headers.h"
#include "message_body.h"

/**
 * @brief class wrapper of HTTP response
 *
 * Object of this class is representation http response
 */
class Response {
    HTTP::Version version;
    int status;
    Headers * headers;
    MessageBody * body;
public:
    /**
     * Create empty response with code status 501 and http version HTTP_UNDEFINED
     */
    Response();

    /**
     * Create response and fill it with declared arguments
     *
     * @param version
     *      http version of response
     * @param status
     *      http response code status
     * @param headers
     *      http response headers as Headers object
     * @param body
     *      http response body as MessageBody object
     */
    Response(HTTP::Version version, int status, Headers & headers, MessageBody & body);

    /**
     * deletes Headers and MessageBody objects
     */
    ~Response();

    /**
     * Set to response http version
     *
     * @param version
     *      value of enum HTTP::Version, which represents http version.
     */
    void setVersion(HTTP::Version version);

    /**
     * Set to response http status
     *
     * @param status
     *      http code status
     */
    void setStatus(int status);

    /**
     * Set to response Headers object and deleting previous one
     *
     * @param headers
     *      headers object
     */
    void setHeaders(Headers & headers);

    /**
     * Set to response MessageBody object and deleting previous one
     *
     * @param body
     *      MessageBody object
     */
    void setBody(MessageBody & body);

    /**
     * get http version of response
     *
     * @return
     *      http response version
     */
    HTTP::Version getVersion();

    /**
     * get http code status of response
     *
     * @return
     *      http response code status
     */
    int getStatus();

    /**
     * get headers of http response
     *
     * @return
     *      http response headers as Headers object
     */
    Headers * getHeaders();

    /**
     * get body of http response
     *
     * @return
     *      http response body as MessageBody object
     */
    MessageBody * getBody();
};
