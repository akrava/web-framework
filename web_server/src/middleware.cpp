#include <middleware.h>

using namespace std;

Middleware::Middleware(const char * nameID) {
    this->request = nullptr;
    this->response = nullptr;
    this->nameID = nameID;
    map = new unordered_map<string, string>();
}

std::string Middleware::getNameID() {
    return nameID;
}

void Middleware::setContent(Request * request, Response * response) {
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

bool Middleware::getValueFromMap(const char * key, string & value) {
    auto iterator = map->find(key);
    if (iterator == map->end()) return false;
    value = iterator->second;
    return true;
}

unordered_map<string, string> * Middleware::getMap() {
    return map;
}

Middleware::~Middleware() {
    delete map;
    map = nullptr;
}
