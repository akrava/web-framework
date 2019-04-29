#include "cookie_entity.h"
#include "parser_http.h"

using namespace std;

CookieEntity::CookieEntity(const char * value, time_t expires, size_t maxAge_sec,
                           const char * domain, const char * path, bool httpOnly) {
    this->value = value;
    this->expires = expires;
    this->maxAge_sec = maxAge_sec;
    this->domain = domain ? domain : std::string();
    this->path = path ? path : std::string();
    this->httpOnly = httpOnly;
}

std::string CookieEntity::toString() {
    std::string result = ParserHTTP::urlEncode(value);
    if (expires != -1) {
        result += "; Expires=";
        tm * ltm = std::localtime(&expires);
        char buff[100];
        if (strftime(buff, sizeof(buff), "%a, %d %b %Y %H:%M:%S GMT", ltm) > 0) {
            result += buff;
        }
    }
    if (maxAge_sec != std::string::npos) {
        result += "; Max-Age=" + to_string(maxAge_sec);
    }
    if (!domain.empty()) {
        result += "; Domain=" + ParserHTTP::urlEncode(domain);
    }
    if (!path.empty()) {
        result += "; Path=" + ParserHTTP::urlEncode(path);
    }
    if (httpOnly) {
        result += "; HttpOnly";
    }
    return result;
}

unique_ptr<Entity> CookieEntity::clone() {
    return make_unique<CookieEntity>(*this);
}

void CookieEntity::setValue(std::string value) {
    this->value = value;
}
