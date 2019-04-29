#pragma once

#include <vector>
#include "request.h"
#include "response.h"
#include "middleware.h"

/**
 * @brief This class is wrapper for important data (like Response, DB, Middleware etc.),
 *      which is needed to handlers
 *
 * This class collects a data about current request, which was parsed from str, have a pointer
 *      to Response object, which will be serialized to client in future (here could be some
 *      written data from previous responses), also there are references to all added middleware
 *      (you could get some by id) and database, which is ready to perform method exec
 */
class Context {
    Request * request;
    Response * response;
    std::vector<Middleware *> * middlewareList;
    bool closeApp;
public:
    /**
     * Constructor create an object of this class: creating Request and Response objects,
     *      and setting NULL to db and middlewareList
     */
    Context();

    /**
     * Deleting Request, Response and DB objects, if they are not NULL
     */
    ~Context();

    /**
     * Gives current request
     *
     * @return
     *      object of Request class
     */
    Request * getRequest();

    /**
     * Gives current response. Could be modified by previous handlers
     *
     * @return
     *      object of Response class
     */
    Response * getResponse();

    /**
     * Deleting existing Request and setting new one
     *
     * @param request
     *      object of Request class (could be inherited)
     */
    void setRequest(Request * request);

    /**
     * Deleting existing Response and setting new one
     *
     * @param response
     *      object of Response class (could be inherited)
     */
    void setResponse(Response * response);

    /**
     * sets vector of Middleware objects, which can be accessed by handlers
     *
     * @param middlewareList
     *      std::vector of Middleware objects
     */
    void setMiddlewareList(std::vector<Middleware *> * middlewareList);

    /**
     * Set permanent (code 301) redirect headers to Response
     *
     * @param uri
     *      destination uri, where current request will be redirected
     */
    void setPermanentlyRedirect(const char * uri);

    /**
     * Set temporary (code 302) redirect headers to Response
     *
     * @param uri
     *      destination uri, where current request will be redirected
     */
    void setTemporaryRedirect(const char * uri);

    /**
     * Set redirect headers to Response
     *
     * @param uri
     *      destination uri, where current request will be redirected
     * @param code
     *      http code of redirect
     */
    void setRedirect(const char * uri, int code);

    /**
     * Method returns added Middleware by id (in string)
     *
     * @param nameID
     *      id of middleware, which was set at startup
     * @return
     *      object of Middleware (could be inherited)
     */
    Middleware * getMiddlewareByNameID(const char * nameID);

    /**
     * Emit signal to stop executing operation. Handler, which used this will
     *      be last executed handler in app
     */
    void emitCloseEvent();

    /**
     * Checks, if handlers emitted CloseEvent
     *
     * @return
     *      true, if there were emitted close event, false otherwise
     */
    bool isClosed();
};
