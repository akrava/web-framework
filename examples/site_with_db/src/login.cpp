#include <akrava/web-server/file_handler.h>
#include <login.h>
#include <akrava/web-server/html_middleware.h>
#include "config.h"

using namespace std;

void HandlerLogin::exec() {
    Middleware * middleware = getContext()->getMiddlewareByNameID("html");
    if (!middleware) return;
    auto * html = (HtmlMiddleware *) (void *) middleware;
    if (html->getView().empty()) return;
    auto body = html->getContext()->find("content");
    string login_order;
    if (!FileHandler::loadFile(__PATH_TO_TEMPLATES"login.mustache", login_order)) return;
    mstch::map order_content;
    string content = mstch::render(login_order, order_content);
    body->second = content;
    html->exec();
}