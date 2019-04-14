#pragma once

#include <string>
#include "context.h"

/**
 * @brief object of this class executes every time on new request, this object (and others)
 *      construct response to client
 *
 * Handler object can be common (will execute on every response) or adjusted to some specified
 *      uri path. It can get all info about request, use added middleware, and make response
 */
class Handler {
    bool hasRoute;
    Context * context;
    std::string route;
    HTTP::Method method;
    Handler * next;
protected:
    /**
     * get current context
     *
     * @return
     *      current Context object
     */
    Context * getContext();
public:
    /**
     * create handler with params: common or routed one
     *
     * @param route
     *      uri path route, if null - handler will be common
     * @param method
     *      uri method, if ANY will be executed on any methods
     */
    explicit Handler(const char * route = nullptr, HTTP::Method method = HTTP::Method::ANY, Handler * next = nullptr);

    /**
     * destructs local variables
     */
    virtual ~Handler() = default;

    /**
     * @brief 
     * 
     * @return true 
     * @return false 
     */
    void setNext(Handler * next);

    /**
     * @brief 
     * 
     * @return true 
     * @return false 
     */
    void add(Handler * handler);

    /**
     *
     */
    void cleanNextHandlers();

    /**
     * check, if route is set
     *
     * @return
     *      true, if handler for specified route, false if it's common one
     */
    bool isRouted();

    /**
     *  get route of handler
     *
     * @return
     *      uri http route path
     */
    std::string getRoute();

    /**
     * get Context object
     *
     * @return
     *      current Context object
     */
    HTTP::Method getMethod();

    /**
     * set Context object
     *
     * @param context
     *      Context object
     */
    void setContext(Context * context);

    /**
     *
     */
    void handleRequest();

    /**
     * this method will be executed on every request (or uri path if set)
     */
    virtual void exec() = 0;
private:
    /**
     *
     * @return
     */
    bool shouldHandleRequest();
};
