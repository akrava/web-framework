#include <parser_http.h>
#include "uri.h"

using namespace std;

URI::URI() {
    uri = string();
    raw_data = string();
    params =  unordered_map<string, string>();
}
//
//URI::URI(const char * str) {
//    params =  unordered_map<string, string>();
//    if (str) {
//        raw_data = string(str);
//        uri = string(str);
//    } else  {
//        raw_data = string();
//        uri = string();
//    }
//}

URI::URI(std::string & uri) {
    raw_data = uri;
    setParamsAndUri(uri);
}

string URI::getUri() {
    return uri;
}

unordered_map<std::string, std::string> URI::getParams() {
    return params;
}

void URI::setParamsAndUri(std::string &uri) {
    string path;
    size_t startParamsPos = uri.find('?');
    if (startParamsPos == string::npos) {
        path = ParserHTTP::urlDecode(uri);
    } else {
        path = uri.substr(0, startParamsPos);
        path = ParserHTTP::urlDecode(path);
        size_t startKeyPos = startParamsPos + 1;
        while  (startKeyPos < uri.length()) {
            size_t endKeyPos = uri.find('=', startKeyPos);
            if (endKeyPos == string::npos) break;
            string key = uri.substr(startKeyPos, endKeyPos - startKeyPos);
            size_t endValuePos = uri.find('&', endKeyPos + 1);
            if (endValuePos == string::npos) {
                endValuePos = uri.length();
            }
            string value = uri.substr(endKeyPos + 1, endValuePos - endKeyPos - 1);
            key = ParserHTTP::urlDecode(key);
            value = ParserHTTP::urlDecode(value);
            params.insert({key, value});
            startKeyPos = endValuePos + 1;
        }
    }
    this->uri = path;
}

bool URI::getValueFromParam(const char *key, std::string &value) {
    auto iterator = params.find(key);
    if (iterator == params.end()) return false;
    value = iterator->second;
    return true;
}

std::string URI::getRawData() {
    return raw_data;
}
