#include "middleware.h"

Middleware::Middleware(const char * nameID, Request * request, Response * response) {
    this->request = request;
    this->response = response;
    this->nameID = nameID;
    map = new std::unordered_map<std::string, std::string>();
}

std::string Middleware::getNameID() {
    return nameID;
}

void Middleware::setContent(Request *request, Response *response) {
    this->request = request;
    this->response = response;
}

void Middleware::addValueToMap(const char * key, const char * value) {
    auto iterator = map->find(key);
    if (iterator != map->end()) {
        iterator->second = value;
        return;
    }
    map->insert({key, value});
}

bool Middleware::getValueFromMap(const char * key, std::string & value) {
    auto iterator = map->find(key);
    if (iterator == map->end()) return false;
    value = iterator->second;
    return true;
}

std::unordered_map<std::string, std::string> * Middleware::getMap() {
    return map;
}

Middleware::~Middleware() {
    delete map;
    map = nullptr;
}
