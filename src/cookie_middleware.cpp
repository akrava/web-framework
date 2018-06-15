#include <cookie_middleware.h>

using namespace std;

CookieMiddleware::CookieMiddleware(const char * nameID) : Middleware (nameID) {
    this->responseCookies = unordered_map<string, CookieEntity>();
}

bool CookieMiddleware::autoExec() {
    map->clear();
    responseCookies.clear();
    for (auto & it : request->getHeaders()->getHeaders()) {
        if (it.first == "Cookie") return true;
    }
    return false;
}

void CookieMiddleware::addCookie(const char *key, CookieEntity &value) {
    auto iterator = responseCookies.find(key);
    if (iterator != responseCookies.end()) {
        responseCookies.erase(iterator);
    }
    responseCookies.insert({key, value});
}

void CookieMiddleware::exec() {
    map->clear();
    string values;
    if (!request->getHeaders()->getValue("Cookie", values)) return;
    size_t startKeyPos = 0;
    while (startKeyPos < values.length()) {
        size_t endKeyPos = values.find('=', startKeyPos);
        if (endKeyPos == string::npos) return;
        string key = values.substr(startKeyPos, endKeyPos - startKeyPos);
        size_t endValuePos = values.find("; ", endKeyPos + 1);
        if (endValuePos == string::npos)  {
            endValuePos = values.length();
        }
        string value = values.substr(endKeyPos + 1, endValuePos - endKeyPos - 1);
        map->insert({key, value});
        startKeyPos = endValuePos + 2;
    }
}

void CookieMiddleware::insertInResponse() {
    string value;
    bool isFirst = true;
    for (auto & it : responseCookies) {
        if (!isFirst) {
            value += "\r\nSet-Cookie: ";
        }
        value += it.first + '=' + it.second.toString();
        isFirst = false;
    }
    if (!responseCookies.empty()) response->getHeaders()->add("Set-Cookie", value.c_str());
}
