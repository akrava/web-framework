#include "auth.h"
#include <akrava/web-server/html_middleware.h>
#include <akrava/web-server/file_handler.h>
#include <akrava/web-server/form_middleware.h>
#include <akrava/web-server/auth_middleware.h>
#include <database_middleware.h>
#include <config.h>

using namespace std;

void AuthHandler::exec() {
    auto auth = (AuthMiddleware *) getContext()->getMiddlewareByNameID("auth");
    if (!auth) return;

    auto * html = (HtmlMiddleware *) (void *)  getContext()->getMiddlewareByNameID("html");
    if (html->getView().empty()) return;

    Middleware * middleware_form = getContext()->getMiddlewareByNameID("form");
    if (!middleware_form) return;
    auto form = (FormMiddleware *) (void *) middleware_form;

    bool error = false;
    string username;
    string pass;

    if (!form->getValueFromMap("login", username)) error = true;
    if (!form->getValueFromMap("password", pass)) error = true;

    if (auth->login(username, pass)) {
        auto res = getContext()->getResponse();
        res->setStatus(302);
        res->setVersion(HTTP::Version::HTTP_1_1);
        res->getHeaders()->add("Location", "/");
        return;
    }

    string template_info;
    if (!FileHandler::loadFile((currentDir() + __PATH_TO_TEMPLATES"/info.mustache").c_str(), template_info)) return;
    mstch::map info_content{{"header", string{"Неправильний пароль або логін"}}};


    string msg = "<p><b>Пароль або логін був введений невірно</b></p>";
    info_content.insert({"content", msg});
    auto body = html->getContext()->find("content");
    string content = mstch::render(template_info, info_content);
    body->second = content;
    html->exec();
}
