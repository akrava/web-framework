#pragma once

#include <middleware.h>
#include <mstch/mstch.hpp>

/**
 *
 */
class HtmlMiddleware : public Middleware {
    std::string view;
    mstch::map * context;
public:
    /**
     *
     * @param nameID
     * @param request
     * @param response
     */
    HtmlMiddleware(const char * nameID, Request * request, Response * response);


    ~HtmlMiddleware();
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
    mstch::map * getContext();

    /**
     *
     * @param view
     */
    void setView(std::string & view);

    /**
     *
     * @return
     */
    std::string getView();
};
