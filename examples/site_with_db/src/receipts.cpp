#include "receipts.h"
#include <akrava/web-server/html_middleware.h>
#include <akrava/web-server/file_handler.h>
#include <database_middleware.h>
#include <config.h>

using namespace std;

void HandlerReceipts::exec() {
    Middleware * middleware = getContext()->getMiddlewareByNameID("html");
    DatabaseMiddleware * db = (DatabaseMiddleware *) getContext()->getMiddlewareByNameID("db");
    if (!middleware) return;
    auto * html = (HtmlMiddleware *) (void *) middleware;
    if (html->getView().empty()) return;
    auto body = html->getContext()->find("content");

    string template_cur;
    if (!FileHandler::loadFile((currentDir() + __PATH_TO_TEMPLATES"/receipts.mustache").c_str(), template_cur)) return;
    mstch::map map_content;

    string curTarget;

    vector<vector<string>> result;
    if (!db->execQuery(
        "SELECT num, sender, receiver, price FROM receipts",
        result,
        nullptr,
        0)
    ) return;

    mstch::array array_table;
    for (auto & it : result) {
        if (it.size() != 4) continue;
        mstch::map cur{
            {"num", it[0]},
            {"sender", it[1]},
            {"receiver", it[2]},
            {"price", it[3]}
        };
        array_table.push_back(cur);
    }

    map_content.insert({"orders", array_table});
    string content = mstch::render(template_cur, map_content);
    body->second = content;
    html->exec();
}
