#pragma once

#include <string>
#include <context.h>
#include <uri.h>
#include <http.h>

/**
 *
 */
class Handler {
    bool hasRoute;
    Context * context;
    std::string route;
    HTTP::Method method;
protected:
    /**
     *
     * @return
     */
    Context * getContext();
public:
    /**
     *
     * @param route
     * @param method
     */
    Handler(const char * route = nullptr, HTTP::Method method = HTTP::Method::ANY);

    virtual ~Handler() {}
    /**
     *
     * @return
     */
    bool isRouted();

    /**
     *
     * @return
     */
    std::string getRoute();

    HTTP::Method getMethod();

    /**
     *
     * @param context
     */
    void setContext(Context * context);

    /**
     *
     */
    virtual void exec() = 0;
};
