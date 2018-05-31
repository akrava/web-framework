#include "middleware.h"

Middleware::Middleware(const char * nameID, Request * request, Response * response) {
    this->request = request;
    this->response = response;
    this->nameID = nameID;
    map = std::unordered_map<std::string, std::string>();
}

std::string Middleware::getNameID() {
    return nameID;
}

void Middleware::setContent(Request *request, Response *response) {
    this->request = request;
    this->response = response;
}
