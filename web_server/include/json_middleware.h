#pragma once

#include <middleware.h>
#include <nlohmann/json.hpp>

/**
 *
 */
class JsonMiddleware : public Middleware {
    nlohmann::json * jsonRequest;
    nlohmann::json * jsonResponse;
    bool errorDeserialize;
public:
    /**
     *
     * @param nameID
     * @param request
     * @param response
     */
    JsonMiddleware(const char * nameID, Request * request, Response * response);

    ~JsonMiddleware();

    /**
     *
     * @return
     */
    bool autoExec();

    /**
     *
     */
    void exec();

    /**
     *
     * @return
     */
    nlohmann::json * getJsonRequest();

    /**
     *
     * @return
     */
    nlohmann::json * getJsonResponse();

    /**
     *
     */
    void fillResponse();
};
