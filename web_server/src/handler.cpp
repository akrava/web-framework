#include "handler.h"

Handler::Handler(const char * route, HTTP::Method method) {
    context = nullptr;
    this->route = route ? std::string(route) : std::string();
    hasRoute = route != nullptr;
    this->method = method;
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

void Handler::setContext(Context *context) {
    this->context = context;
}

HTTP::Method Handler::getMethod() {
    return method;
}
