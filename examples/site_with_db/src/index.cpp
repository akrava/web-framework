#include <string>
#include <index.h>
#include <akrava/web-server/file_handler.h>
#include <akrava/web-server/html_middleware.h>
#include <mstch/mstch.hpp>
#include "database_middleware.h"
#include <config.h>

using namespace std;

void HandlerIndex::exec() {
    Middleware * middleware = getContext()->getMiddlewareByNameID("html");
    DatabaseMiddleware * db = (DatabaseMiddleware *) getContext()->getMiddlewareByNameID("db");
    if (!middleware) return;
    auto * html = (HtmlMiddleware *) (void *) middleware;
    if (html->getView().empty()) return;

    auto body = html->getContext()->find("content");

    string template_index;
    if (!FileHandler::loadFile(__PATH_TO_TEMPLATES"index.mustache", template_index)) {
        return;
    }
    mstch::map index_content;
    index_content.insert({"header", string{"Logistic company #1"}});

    vector<vector<string>> result_text;
    if (!db->execQuery(
            "SELECT data FROM site WHERE value = 'index_text'",
            result_text,
            nullptr,
            0)
    ) return;
    if (result_text.size() != 1 || result_text[0].size() != 1) return;
    index_content.insert({"text", result_text[0][0]});

    string content = mstch::render(template_index, index_content);
    body->second = content;
    html->exec();
}
