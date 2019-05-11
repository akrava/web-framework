#include "receipts_add.h"
#include <akrava/web-server/html_middleware.h>
#include <akrava/web-server/file_handler.h>
#include <akrava/web-server/cookie_middleware.h>
#include <akrava/web-server/form_middleware.h>
#include <akrava/web-server/parser_http.h>
#include <database_middleware.h>
#include <config.h>

using namespace std;

void HandlerReceiptsAdd::exec() {
    Request * request = getContext()->getRequest();
    Middleware * middleware = getContext()->getMiddlewareByNameID("html");
    auto * db = dynamic_cast<DatabaseMiddleware *>(getContext()->getMiddlewareByNameID("db"));
    if (!middleware) return;
    auto * html = dynamic_cast<HtmlMiddleware *>(middleware);
    if (html->getView().empty()) return;
    string number_str;

    Middleware * middleware_form = getContext()->getMiddlewareByNameID("form");
    if (!middleware_form) return;

    auto * form = dynamic_cast<FormMiddleware *>(middleware_form);

    bool error = false;

    string recipient_str;
    string receiver_str;
    string departure_date_str;
    string arrival_date_str;
    string departure_str;
    string cost_str;

    string new_num;



    if (!form->getValueFromMap("recipient", recipient_str)) error = true;
    if (!form->getValueFromMap("receiver", receiver_str)) error = true;
    if (!form->getValueFromMap("departure_date", departure_date_str)) error = true;
    if (!form->getValueFromMap("arrival_date", arrival_date_str)) error = true;
    if (!form->getValueFromMap("departure", departure_str)) error = true;
    if (!form->getValueFromMap("cost", cost_str)) error = true;

    time_t time_departure = toTime_t(departure_date_str);
    time_t time_arrival = toTime_t(arrival_date_str);

    if (!error) {
        vector<vector<string>> num_str;
        int num;
        if (!db->execQuery(
                "SELECT MAX(num) FROM receipts",
                num_str,
                nullptr,
                0)
        ) return;

        try {
            num = stoi(num_str.at(0).at(0));
        }
        catch (out_of_range & err) {
        }
        catch (invalid_argument & err) {
        }

        new_num = to_string(++num);

        const char * data[] = {
                new_num.c_str(),
                recipient_str.c_str(),
                receiver_str.c_str(),
                (to_string(time_departure)).c_str(),
                (to_string(time_arrival)).c_str(),
                departure_str.c_str(),
                cost_str.c_str()
        };

        vector<vector<string>> res_query;
        if (!db->execQuery(
                "INSERT INTO receipts ( `num`,`sender`,`receiver`, `date_dep`, `date_arr`, `city_id`, `price`) "
                "VALUES (?, ?, ?, ?, ?, ?, ?);",
                res_query,
                (char **)data,
                7)
        ) return;

        if (!res_query.empty()) error = true;


    }




    string template_info;
    if (!FileHandler::loadFile(__PATH_TO_TEMPLATES"info.mustache", template_info)) return;
    mstch::map info_content{{"header", string{"Створення відправлення"}}};

    if (error) {
        info_content.insert({
            "content",
            string{"<span class = \"error\">Не вдалося створити, сталася помилка.</span><p><a href=\"/receipts\">Повернутися</a> до списку</p>"}
        });
    } else {
        info_content.insert({
            "content",
            string{"<p>Відправлення <b>" + new_num + "</b> було створено!</p><p><a href=\"/receipts\">Повернутися</a> до списку</p>"}
        });
    }
    auto body = html->getContext()->find("content");
    string content = mstch::render(template_info, info_content);
    body->second = content;
    html->exec();
}
