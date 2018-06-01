//#include <cookie_middleware.h>
//#include <response.h>
//#include <request.h>
#include "cookie_middleware.h"
#include <middleware.h>

using namespace std;

CookieMiddleware::CookieMiddleware(const char * nameID, Request * request, Response * response)
                                        : Middleware (nameID, request, response) {
    this->responseCookies = std::unordered_map<string, CookieEntity>();
}

bool CookieMiddleware::autoExec() {
    for (auto & it : request->getHeaders()->getHeaders()) {
        if (it.first == "Cookie") return true;
    }
    return false;
}

void CookieMiddleware::addCooike(const char * key, CookieEntity & value) {
    auto iterator = responseCookies.find(key);
    if (iterator != responseCookies.end()) {
        responseCookies.erase(iterator);
    }
    responseCookies.insert({key, value});
}

void CookieMiddleware::exec() {
    map->clear();
    std::string values;
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
    for (auto & it : responseCookies) {
        std::string value = it.first + '=' + it.second.toString();
        response->getHeaders()->add("Set-Cookie", value.c_str());
    }
    map->clear();
}
