#include <context.h>

using namespace std;

Context::Context()  {
    closeApp = false;
    request = new Request();
    response = new Response();
    middlewareList = nullptr;
    db = nullptr;
}

Request * Context::getRequest() {
    return request;
}

void Context::setDB(DBManager * db) {
    delete this->db;
    this->db = db;
}

DBManager * Context::getDB() {
    return db;
}

Response * Context::getResponse() {
    return response;
}

void Context::setRequest(Request * request) {
    delete this->request;
    this->request = request;
}

void Context::setResponse(Response * response) {
    delete this->response;
    this->response = response;
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

void Context::setMiddlewareList(vector<Middleware *> *middlewareList) {
    delete this->middlewareList;
    this->middlewareList = middlewareList;
}

void Context::emitCloseEvent() {
    closeApp = true;
}

bool Context::isClosed() {
    return closeApp;
}

Context::~Context() {
    delete request;
    delete response;
    delete db;
    request = nullptr;
    response = nullptr;
    db = nullptr;
}
