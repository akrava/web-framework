#include <parser_http.h>
#include <form_middleware.h>

using namespace std;

bool FormMiddleware::autoExec() {
    std::string value;
    if (request->getHeaders()->getValue("Content-Type", value)) {
        return value.find("application/x-www-form-urlencoded") == 0;
    } else {
        return false;
    }
}

void FormMiddleware::exec() {
    map->clear();
    string body = request->getMessageBody()->getBody();
    size_t startKeyPos = 0;
    while  (startKeyPos < body.length()) {
        size_t endKeyPos = body.find('=', startKeyPos);
        if (endKeyPos == string::npos) break;
        string key = body.substr(startKeyPos, endKeyPos - startKeyPos);
        size_t endValuePos = body.find('&', endKeyPos + 1);
        if (endValuePos == string::npos) {
            endValuePos = body.length();
        }
        string value = body.substr(endKeyPos + 1, endValuePos - endKeyPos - 1);
        key = ParserHTTP::urlDecode(key);
        value = ParserHTTP::urlDecode(value);
        map->insert({key, value});
        startKeyPos = endValuePos + 1;
    }
}
