#include "receipts_delete.h"
#include <akrava/web-server/html_middleware.h>
#include <akrava/web-server/file_handler.h>
#include <akrava/web-server/cookie_middleware.h>
#include <akrava/web-server/parser_http.h>
#include <database_middleware.h>
#include <config.h>

using namespace std;

void HandlerReceiptsDelete::exec() {
    Request * request = getContext()->getRequest();
    Middleware * middleware = getContext()->getMiddlewareByNameID("html");
    auto * db = dynamic_cast<DatabaseMiddleware *>(getContext()->getMiddlewareByNameID("db"));
    if (!middleware) return;
    auto * html = dynamic_cast<HtmlMiddleware *>(middleware);
    if (html->getView().empty()) return;
    string number_str;

    bool error = !request->getURI()->getValueFromParam("num", number_str);

    if (!error) {
        char *data[] = {(char *) number_str.c_str()};
        vector<vector<string>> res_query;
        db->execQuery(
                "DELETE FROM receipts WHERE num = ?",
                res_query,
                (char **) data,
                1);
    }


    string template_info;
    if (!FileHandler::loadFile(__PATH_TO_TEMPLATES"info.mustache", template_info)) return;
    mstch::map info_content{{"header", string{"Видалення об'єкта"}}};

    if (error) {
        info_content.insert({
            "content",
            string{"<span class = \"error\">Не вдалося видалити, сталася помилка.</span><p><a href=\"/receipts\">Повернутися</a> до списку</p>"}
        });
    } else {
        info_content.insert({
            "content",
            string{"<p>Відправлення <b>" + number_str + "</b> було видалено!</p><p><a href=\"/receipts\">Повернутися</a> до списку</p>"}
        });
    }
    auto body = html->getContext()->find("content");
    string content = mstch::render(template_info, info_content);
    body->second = content;
    html->exec();
}
