#include "receipts_change.h"
#include <akrava/web-server/html_middleware.h>
#include <akrava/web-server/file_handler.h>
#include <akrava/web-server/cookie_middleware.h>
#include <akrava/web-server/parser_http.h>
#include <database_middleware.h>
#include <config.h>

using namespace std;

void HandlerReceiptsChange::exec() {
    Request * request = getContext()->getRequest();
    Middleware * middleware = getContext()->getMiddlewareByNameID("html");
    auto * db = dynamic_cast<DatabaseMiddleware *>(getContext()->getMiddlewareByNameID("db"));
    if (!middleware) return;
    auto * html = dynamic_cast<HtmlMiddleware *>(middleware);
    if (html->getView().empty()) return;

    string number_str;

    bool error = !request->getURI()->getValueFromParam("num", number_str);
    if (error) return;

    string template_info;
    if (!FileHandler::loadFile(__PATH_TO_TEMPLATES"receipt.mustache", template_info)) return;
    mstch::map info_content{{"header", string{"Редагування відправлення"}}};

    info_content.insert({"uri_action", string{"/receipts/edit"}});

    vector<vector<string>> result_cities;
    if (!db->execQuery(
            "SELECT id, name FROM cities",
            result_cities,
            nullptr,
            0)
    ) return;

    vector<vector<string>> result_receipt;
    char *data[] = {(char *) number_str.c_str()};

    if (!db->execQuery(
            "SELECT sender, receiver, date_dep, date_arr, city_id, price FROM receipts WHERE num = ?",
            result_receipt,
            (char **) data,
            1)
    ) return;

    mstch::array array_table_cities;
    for (auto & it : result_cities) {
        if (it.size() != 2) continue;
        mstch::map cur{{"id", it[0]}, {"name", it[1]}};
        if (it[0] == result_receipt[0][4]) {
            cur.insert({"selected", true});
        }
        array_table_cities.push_back(cur);
    }

    time_t dep = stoi(result_receipt[0][2]);
    result_receipt[0][2] = ParserHTTP::getTime(&dep, "%Y-%m-%d");

    time_t arr = stoi(result_receipt[0][3]);
    result_receipt[0][3] = ParserHTTP::getTime(&arr, "%Y-%m-%d");

    info_content.insert({"cities", array_table_cities});
    info_content.insert({"recipient", result_receipt[0][0]});
    info_content.insert({"receiver", result_receipt[0][1]});
    info_content.insert({"departure_date", result_receipt[0][2]});
    info_content.insert({"arrival_date", result_receipt[0][3]});
    info_content.insert({"cost", result_receipt[0][5]});
    info_content.insert({"num", number_str});

    auto body = html->getContext()->find("content");
    string content = mstch::render(template_info, info_content);
    body->second = content;
    html->exec();
}
