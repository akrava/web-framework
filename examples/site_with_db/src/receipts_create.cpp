#include "receipts_create.h"
#include <akrava/web-server/html_middleware.h>
#include <akrava/web-server/file_handler.h>
#include <akrava/web-server/cookie_middleware.h>
#include <akrava/web-server/parser_http.h>
#include <database_middleware.h>
#include <config.h>

using namespace std;

void HandlerReceiptsCreate::exec() {
    Request * request = getContext()->getRequest();
    Middleware * middleware = getContext()->getMiddlewareByNameID("html");
    auto * db = dynamic_cast<DatabaseMiddleware *>(getContext()->getMiddlewareByNameID("db"));
    if (!middleware) return;
    auto * html = dynamic_cast<HtmlMiddleware *>(middleware);
    if (html->getView().empty()) return;

    string template_info;
    if (!FileHandler::loadFile(__PATH_TO_TEMPLATES"receipt.mustache", template_info)) return;
    mstch::map info_content{{"header", string{"Створення відправлення"}}};

    info_content.insert({"uri_action", string{"/receipts/add"}});

    vector<vector<string>> result_cities;
    if (!db->execQuery(
            "SELECT id, name FROM cities",
            result_cities,
            nullptr,
            0)
    ) return;

    mstch::array array_table_cities;
    for (auto & it : result_cities) {
        if (it.size() != 2) continue;
        mstch::map cur{{"id", it[0]}, {"name", it[1]}};
        array_table_cities.push_back(cur);
    }
    info_content.insert({"cities", array_table_cities});
    auto body = html->getContext()->find("content");
    string content = mstch::render(template_info, info_content);
    body->second = content;
    html->exec();
}
