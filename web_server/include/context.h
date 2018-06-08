#pragma once

#include <vector>
#include <request.h>
#include <response.h>
#include <middleware.h>
#include <db_manager.h>


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
    DBManager * db;
    bool closeApp;
public:
    /**
     *
     */
    Context();

    ~Context();

    /**
     *
     * @param db
     */
    void setDB(DBManager * db);

    /**
     *
     * @return
     */
    DBManager * getDB();

    /**
     *
     * @return
     */
    Request * getRequest();

    /**
     *
     * @return
     */
    Response * getResponse();

    /**
     *
     * @param request
     */
    void setRequest(Request * request);

    /**
     *
     * @param response
     */
    void setResponse(Response * response);

    /**
     *
     * @param middlewareList
     */
    void setMiddlewareList(std::vector<Middleware *> * middlewareList);

    /**
     *
     * @param uri
     */
    void setPermanentlyRedirect(const char * uri);

    /**
     *
     * @param uri
     */
    void setTemporaryRedirect(const char * uri);

    /**
     *
     * @param uri
     * @param code
     */
    void setRedirect(const char * uri, int code);

    /**
     *
     * @param nameID
     * @return
     */
    Middleware * getMiddlewareByNameID(const char * nameID);

    void emmitCloseEvent();

    bool isClosed();
};
