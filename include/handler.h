#pragma once
#include <string>
#include <context.h>
#include <uri.h>
#include <http.h>


class Handler {
    bool hasRoute;
    Context * context;
    std::string route;
    HTTP::Method method;
protected:
    Context * getContext();
public:
    Handler(const char * route = nullptr, HTTP::Method method1 = HTTP::Method::ANY);
    bool isRouted();
    std::string getRoute();
    void setContext(Context * context);
    virtual void exec() = 0;
};



