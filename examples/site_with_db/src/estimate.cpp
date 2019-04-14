#include <estimate.h>
#include <akrava/web-server/html_middleware.h>
#include <akrava/web-server/file_handler.h>
#include <database_middleware.h>
#include <config.h>

using namespace std;

void HandlerEstimate::exec() {
    Middleware * middleware = getContext()->getMiddlewareByNameID("html");
    DatabaseMiddleware * db = (DatabaseMiddleware *) getContext()->getMiddlewareByNameID("db");
    if (!middleware) return;
    auto * html = (HtmlMiddleware *) (void *) middleware;
    if (html->getView().empty()) return;
    auto body = html->getContext()->find("content");

    string template_calculate;
    if (!FileHandler::loadFile((currentDir() + __PATH_TO_TEMPLATES"/estimate.mustache").c_str(), template_calculate)) return;
    mstch::map calculate_content;

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

    vector<vector<string>> result_uri;
    if (!db->execQuery(
            "SELECT data FROM site WHERE value = 'uri_estimate'",
            result_uri,
            nullptr,
            0)
            ) return;
    if (result_uri.size() != 1 || result_uri[0].size() != 1) return;

    calculate_content.insert({"uri_estimate", result_uri[0][0]});
    calculate_content.insert({"cities", array_table_cities});
    string content = mstch::render(template_calculate, calculate_content);
    body->second = content;
    html->exec();
    Handler::exec();
}
