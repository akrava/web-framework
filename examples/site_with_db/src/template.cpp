#include <template.h>
#include <akrava/web-server/html_middleware.h>
#include <akrava/web-server/file_handler.h>
#include <config.h>

using namespace std;

void HandlerTemplate::exec() {
    getContext()->getResponse()->getHeaders()->add("Content-Type", "text/html; charset=utf-8");
    string template_str;
    bool found_template = FileHandler::loadFile(__PATH_TO_TEMPLATES"common.mustache", template_str);
    string data = "<html><head><title>";
    data += to_string(getContext()->getResponse()->getStatus());
    data += " " + HTTP::getReasonPhrase(getContext()->getResponse()->getStatus());
    data += "</title></head><body><h1>";
    data += to_string(getContext()->getResponse()->getStatus()) + " ";
    data += HTTP::getReasonPhrase(getContext()->getResponse()->getStatus());
    data += "</h1><hr><address>Web-framework (c++) 2018</address></body></html>";
    getContext()->getResponse()->getBody()->setBody(data);
    if (found_template) {
        Middleware * middleware = getContext()->getMiddlewareByNameID("html");
        if (!middleware) return;
        auto * html = (HtmlMiddleware *) (void *) middleware;
        html->setView(template_str);
    }
}