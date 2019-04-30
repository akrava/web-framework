#pragma once

#include "middleware.h"
#include <mstch/mstch.hpp>

/**
 * @brief inherited class to render html pages from templates
 *
 * HtmlMiddleware uses logic-less mustache templates to render html pages
 */
class HtmlMiddleware : public Middleware {
    std::string view;
    mstch::map * context;
public:
    /**
     * create middleware
     *
     * @param nameID
     *      name id
     */
    explicit HtmlMiddleware(const char * nameID);

    /**
     * delete context map, used fot rendering
     */
    ~HtmlMiddleware() override;

    /**
     * Cleanup context map
     *
     * @return
     *      true, if ready to render
     */
    bool autoExec() override;

    /**
     * render template and set to response body
     */
    void exec() override;

    /**
     * get current context map
     *
     * @return
     *      context map of template
     */
    mstch::map * getContext();

    /**
     * set new template view
     *
     * @param view
     *       template view as string
     */
    void setView(std::string & view);

    /**
     * get current template view
     *
     * @return
     *      template view string
     */
    std::string getView();

    /**
     * delete all data, saved in internal values, set to default
     */
     void clear() override;
};
