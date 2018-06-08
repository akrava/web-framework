#pragma once

#include <http.h>
#include <message_body.h>
#include <uri.h>
#include <headers.h>

/**
 * @brief class wrapper of HTTP request
 *
 * Object of this class is deserialized http request, where all consisting data is represented
 *      by objects of another classes
 */
class Request {
    HTTP::Method method;
    URI * uri;
    HTTP::Version version;
    Headers * headers;
    MessageBody * body;
public:
    /**
     * Makes empty Request object, where method and version is undefined
     */
    Request();

    /**
     * Makes Request object with declared arguments
     *
     * @param method
     *      http method of request
     * @param URI
     *      http request uri string, which is used to construct URI object
     * @param version
     *      request http version
     * @param headers
     *      http request headers string, which is used to construct Headers object
     * @param body
     *      http request body string, which is used to construct MessageBody object
     */
    Request(HTTP::Method method, std::string & URI, HTTP::Version version,
            std::string & headers, std::string & body);

    /**
     * deletes URI, Headers and MessageBody objects
     */
    ~Request();

    /**
     * get request method
     *
     * @return
     *      value of enum HTTP::Method, which represents http method.
     */
    HTTP::Method getMethod();

    /**
     * get URI request object
     *
     * @return
     *      request URI object
     */
    URI * getURI();

    /**
     * get http version of request
     *
     * @return
     *      value of enum HTTP:Version, which represents http version.
     */
    HTTP::Version getVersion();

    /**
     * get request headers
     *
     * @return
     *      Headers request object
     */
    Headers * getHeaders();

    /**
     * get request body
     *
     * @return
     *      MessageBody request object
     */
    MessageBody * getMessageBody();
};
