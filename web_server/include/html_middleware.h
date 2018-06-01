#pragma once

#include <middleware.h>
#include <mstch/mstch.hpp>

class HtmlMiddleware : public Middleware {
    std::string view;
    mstch::map * context;
public:
    HtmlMiddleware(const char * nameID, Request * request, Response * response);
    bool autoExec();
    void exec();
    mstch::map * getContext();
    void setView(std::string & view);
    std::string getView();
};


