#include <feedback.h>
#include <akrava/web-server/html_middleware.h>
#include <akrava/web-server/file_handler.h>
#include <database_middleware.h>
#include <config.h>

using namespace std;

void HandlerFeedback::exec() {
    Middleware * middleware = getContext()->getMiddlewareByNameID("html");
    DatabaseMiddleware * db = (DatabaseMiddleware *) getContext()->getMiddlewareByNameID("db");
    if (!middleware) return;
    auto * html = (HtmlMiddleware *) (void *) middleware;
    if (html->getView().empty()) return;

    auto body = html->getContext()->find("content");

    string template_order;
    if (!FileHandler::loadFile(__PATH_TO_TEMPLATES"feedback.mustache", template_order)) return;
    mstch::map order_content;

    vector<vector<string>> result_uri;
    if (!db->execQuery(
            "SELECT data FROM site WHERE value = 'uri_feedback_post'",
            result_uri,
            nullptr,
            0)
    ) return;
    if (result_uri.size() != 1 || result_uri[0].size() != 1) return;

    order_content.insert({"uri_feedback", result_uri[0][0]});

    string content = mstch::render(template_order, order_content);
    body->second = content;
    html->exec();
}