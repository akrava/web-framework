#include "context.h"
#include "response.h"
#include "request.h"

using namespace std;

Context::Context() : response(), request() { this->middlewareList = nullptr; }

Context::Context(Request & request) : request(new Request(request)), response() {  this->middlewareList = nullptr; }

Context::Context(Request & request, Response & response, vector<Middleware *> * middlewareList) : request(new Request(request)), response(new Response(response)) {
    this->middlewareList = middlewareList;
    }

Request * Context::getRequest() {
    return request;
}

Response * Context::getResponse() {
    return response;
}

void Context::setRequest(Request & request) {
    this->request = new Request(request);
}

void Context::setResponse(Response & response) {
    this->response = new Response(response);
}

void Context::setPermanentlyRedirect(const char * uri) {
    setRedirect(uri, 301);
}

void Context::setTemporaryRedirect(const char * uri) {
    setRedirect(uri, 302);
}

void Context::setRedirect(const char * uri, int code) {
    response->setStatus(code);
    response->getHeaders()->add("Location", uri);
}

Middleware * Context::getMiddlewareByNameID(const char *nameID) {
    for (auto * cur : *middlewareList) {
        if (cur->getNameID() == nameID) return cur;
    }
    return nullptr;
}

void Context::setMiddlewareList(std::vector<Middleware *> *middlewareList) {
    this->middlewareList = middlewareList;
}

