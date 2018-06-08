#pragma once

#include <middleware.h>
#include <nlohmann/json.hpp>

/**
 * @brief inherited class to perform any actions with json data
 *
 * JsonMiddleware is intended to parse json from http request, fill response with json
 *      and perform any actions with json
 */
class JsonMiddleware : public Middleware {
    nlohmann::json * jsonRequest;
    nlohmann::json * jsonResponse;
    bool errorDeserialize;
public:
    /**
     * create middleware
     *
     * @param nameID
     *      name id
     */
    JsonMiddleware(const char * nameID);

    /**
     * delete json request and response objects
     */
    ~JsonMiddleware();

    /**
     * Check if request is json data
     *
     * @return
     *      true, if content type of http request is json
     */
    bool autoExec();

    /**
     * parse json from http request
     */
    void exec();

    /**
     * get json request object
     *
     * @return
     *      json request object
     */
    nlohmann::json * getJsonRequest();

    /**
     * get json response object
     *
     * @return
     *      json response object
     */
    nlohmann::json * getJsonResponse();

    /**
     * set response body with serialized json data from jsonResponse
     */
    void fillResponse();
};
