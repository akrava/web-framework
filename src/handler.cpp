#include <handler.h>

using namespace std;

Handler::Handler(const char * route, HTTP::Method method, Handler * next) {
    context = nullptr;
    this->route = route ? string(route) : string();
    hasRoute = route != nullptr;
    this->method = method;
    this->next = next;
}

void Handler::setNext(Handler * next) {
    this->next = next;
}

void Handler::add(Handler * handler) {
    if (next) {
        next->add(handler);
    } else {
        next = handler;
    }
}

void Handler::exec() {
    if (next) {
        next->exec();
    }
}

void Handler::cleanNextHandlers() {
    if (!next) return;
    next->cleanNextHandlers();
    delete next;
}

Context * Handler::getContext() {
    return context;
}

bool Handler::isRouted() {
    return hasRoute;
}

std::string Handler::getRoute() {
    return route;
}

void Handler::setContext(Context * context) {
    this->context = context;
}

HTTP::Method Handler::getMethod() {
    return method;
}
