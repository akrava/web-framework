#include "html_middleware.h"

HtmlMiddleware::HtmlMiddleware(const char * nameID) : Middleware(nameID) {
    view = std::string();
    this->context = new mstch::map();
}

bool HtmlMiddleware::autoExec() {
    context->clear();
    return false;
}

void HtmlMiddleware::exec() {
    std::string rendered = mstch::render(view, *context);
    MessageBody html = MessageBody(rendered);
    response->setBody(html);
}

mstch::map * HtmlMiddleware::getContext() {
    return context;
}

void HtmlMiddleware::setView(std::string & view) {
    this->view = view;
}

std::string HtmlMiddleware::getView() {
    return view;
}

HtmlMiddleware::~HtmlMiddleware() {
    delete context;
}

void HtmlMiddleware::clear() {
    Middleware::clear();
    view.clear();
    if (map) map->clear();
}
