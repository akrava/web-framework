#include "cookie_entity_http.h"
#include "parser_http.h"

using namespace std;

CookieEntityHttp::CookieEntityHttp(const char * value) : value(value), httpOnly(true) { }

string CookieEntityHttp::toString() {
    return ParserHTTP::urlEncode(value) + "; HttpOnly";
}

unique_ptr<Entity> CookieEntityHttp::clone() {
    return make_unique<CookieEntityHttp>(*this);
}

void CookieEntityHttp::setValue(std::string value) {
    this->value = value;
}
