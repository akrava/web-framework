#include <cookie_entity_default.h>
#include <parser_http.h>

using namespace std;

CookieEntityDefault::CookieEntityDefault(const char * value) : value(value) { }

string CookieEntityDefault::toString() {
    return ParserHTTP::urlEncode(value);
}

unique_ptr<Entity> CookieEntityDefault::clone() {
    return make_unique<CookieEntityDefault>(*this);
}

void CookieEntityDefault::setValue(std::string value) {
    this->value = value;
}
