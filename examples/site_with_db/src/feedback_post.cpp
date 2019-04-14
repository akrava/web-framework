#include <feedback_post.h>
#include <akrava/web-server/html_middleware.h>
#include <akrava/web-server/form_middleware.h>
#include <akrava/web-server/file_handler.h>
#include <database_middleware.h>
#include <config.h>

using namespace std;

void HandlerFeedbackPost::exec() {
    Middleware * middleware = getContext()->getMiddlewareByNameID("html");
    DatabaseMiddleware * db = (DatabaseMiddleware *) getContext()->getMiddlewareByNameID("db");
    if (!middleware) return;
    auto * html = (HtmlMiddleware *) (void *) middleware;
    if (html->getView().empty()) return;

    Middleware * middleware_form = getContext()->getMiddlewareByNameID("form");
    if (!middleware_form) return;
    auto * form = (FormMiddleware *) (void *) middleware_form;

    bool error = false;

    string name_str;
    string email_str;
    string comment_str;

    if (!form->getValueFromMap("name", name_str)) error = true;
    if (!form->getValueFromMap("email", email_str)) error = true;
    if (!form->getValueFromMap("comment", comment_str)) error = true;

    const char * data[] = {
            name_str.c_str(),
            email_str.c_str(),
            comment_str.c_str()
    };

    vector<vector<string>> res_query;
    if (!db->execQuery(
            "INSERT INTO reviews ( `name`,`email`,`text`) "
            "VALUES (?, ?, ?);",
            res_query,
            (char **)data,
            3)
    ) return;

    if (!res_query.empty()) error = true;

    string template_info;
    if (!FileHandler::loadFile((currentDir() + __PATH_TO_TEMPLATES"/info.mustache").c_str(), template_info)) return;
    mstch::map info_content{{"header", string{"Feedback"}}};

    if (error) {
        info_content.insert({
                "content",
                string{"<span class = \"error\">Некоректні введені дані! Спробуйте ще раз.</span>"}
        });
    } else {
        info_content.insert({
                "content",
                string{"<p>Дякуємо, <b>" + name_str + "</b>, вашу відгук буде врахован!"}
        });
    }
    auto body = html->getContext()->find("content");
    string content = mstch::render(template_info, info_content);
    body->second = content;
    html->exec();
    Handler::exec();
}