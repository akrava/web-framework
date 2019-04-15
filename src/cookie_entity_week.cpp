#include <cookie_entity_week.h>
#include <parser_http.h>

using namespace std;

static tm operator+ (tm time, int spanInDays);

CookieEntityWeek::CookieEntityWeek(const char * value) : value(value) {
    auto now = time(nullptr) ;
    auto time_now = *gmtime(addressof(now));
    auto in_week = time_now + 7;
    expires = mktime(addressof(in_week));
}

string CookieEntityWeek::toString() {
    string result = ParserHTTP::urlEncode(value);
    result += "; Expires=";
    tm * ltm = localtime(&expires);
    char buff[100];
    if (strftime(buff, sizeof(buff), "%a, %d %b %Y %H:%M:%S GMT", ltm) > 0) {
        result += buff;
    }
    return result;
}

unique_ptr<Entity> CookieEntityWeek::clone() {
    return make_unique<CookieEntityWeek>(*this);
}

void CookieEntityWeek::setValue(std::string value) {
    this->value = value;
}

static tm operator+ (tm time, int spanInDays) {
    time.tm_mday += spanInDays;
    const auto t = mktime(addressof(time));
    return *gmtime(addressof(t));
}