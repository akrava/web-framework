#pragma once

#include "http.h"
#include "message_body.h"
#include <uri.h>
#include <headers.h>

/**
 *
 */
class Request {
    HTTP::Method method;
    URI * uri;
    HTTP::Version version;
    Headers * headers;
    MessageBody * body;
public:
    /**
     *
     */
    Request();

    /**
     *
     * @param method
     * @param URI
     * @param version
     * @param headers
     * @param body
     */
    Request(HTTP::Method method, std::string & URI, HTTP::Version version,
            std::string & headers, std::string & body);


    ~Request();
    /**
     *
     * @return
     */
    HTTP::Method getMethod();

    /**
     *
     * @return
     */
    URI * getURI();

    /**
     *
     * @return
     */
    HTTP::Version getVersion();

    /**
     *
     * @return
     */
    Headers * getHeaders();

    /**
     *
     * @return
     */
    MessageBody * getMessageBody();
};
