#include <order_post.h>
#include <akrava/web-server/html_middleware.h>
#include <akrava/web-server/form_middleware.h>
#include <akrava/web-server/file_handler.h>
#include <database_middleware.h>
#include <akrava/web-server/parser_http.h>
#include <config.h>

using namespace std;

void HandlerOrderPost::exec() {
    Middleware * middleware = getContext()->getMiddlewareByNameID("html");
    DatabaseMiddleware * db = (DatabaseMiddleware *) getContext()->getMiddlewareByNameID("db");
    if (!middleware) return;
    auto * html = (HtmlMiddleware *) (void *) middleware;
    if (html->getView().empty()) return;
    Middleware * middleware_form = getContext()->getMiddlewareByNameID("form");
    if (!middleware_form) return;
    auto * form = (FormMiddleware *) (void *) middleware_form;
    bool error = false;

    string city_str;
    string name_str;
    string email_str;
    string phone_str;
    string address_str;
    string comment_str;

    if (!form->getValueFromMap("city", city_str)) error = true;
    if (!form->getValueFromMap("name", name_str)) error = true;
    if (!form->getValueFromMap("email", email_str)) error = true;
    if (!form->getValueFromMap("phone", phone_str)) error = true;
    if (!form->getValueFromMap("address", address_str)) error = true;
    if (!form->getValueFromMap("comment", comment_str)) error = true;

    string time_cur = ParserHTTP::getTime(nullptr, "%Y-%m-%d %H:%M:%S");

    const char * data[] = {
            city_str.c_str(),
            name_str.c_str(),
            email_str.c_str(),
            phone_str.c_str(),
            address_str.c_str(),
            comment_str.size() == 0 ? "NULL" : comment_str.c_str(),
            time_cur.c_str()
    };

    vector<vector<string>> res_query;
    if (!db->execQuery(
            "INSERT INTO orders (`city_id`, `name`,`email`,`phone`,`address`,`comment`,`date_order`) "
            "VALUES (?, ?, ?, ?, ?, ?, ?);",
            res_query,
            (char **)data,
            7)
    ) return;

    if (!res_query.empty()) error = true;

    string template_info;
    if (!FileHandler::loadFile(__PATH_TO_TEMPLATES"info.mustache", template_info)) return;
    mstch::map info_content{{"header", string{"Результат заповнення заявки"}}};

    if (error) {
        info_content.insert({
                "content",
                string{"<span class = \"error\">Некоректні введені дані! Спробуйте ще раз.</span>"}
        });
    } else {
        info_content.insert({
                "content",
                string{"<p>Дякуємо, <b>" + name_str + "</b>, вашу заявку прийнято!"}
        });
    }
    auto body = html->getContext()->find("content");
    string content = mstch::render(template_info, info_content);
    body->second = content;
    html->exec();
}
