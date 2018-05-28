#include "context.h"
#include "response.h"
#include "request.h"

using namespace std;

Context::Context() : response(), request() { }

Context::Context(Request & request) : request(request), response() { }

Context::Context(Request & request, Response & response) : request(request), response(response) { }

Request Context::getRequest() {
   // return request;
}

Response Context::getResponse() {
    return response;
}

void Context::setRequest(Request & request) {
    this->request = request;
}

void Context::setResponse(Response & response) {
    this->response = response;
}
