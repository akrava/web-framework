#include "headers.h"
#include <string>

using namespace std;


Headers::Headers() {
    headers = unordered_map<string, string>();
}

Headers::Headers(string & httpHeaders) : Headers() {
    size_t startKeyPos = 0;
    while (startKeyPos < httpHeaders.length()) {
        size_t endKeyPos = httpHeaders.find(": ", startKeyPos);
        if (endKeyPos == string::npos) return;
        string key = httpHeaders.substr(startKeyPos, endKeyPos - startKeyPos);
        size_t endValuePos = httpHeaders.find("\r\n", endKeyPos + 2);
        if (endValuePos == string::npos)  {
            endValuePos = httpHeaders.length();
        }
        string value = httpHeaders.substr(endKeyPos + 2, endValuePos - endKeyPos - 2);
        headers.insert({key, value});
        startKeyPos = endValuePos + 2;
    }
}

std::string Headers::toString() {
    string result = string();
    for (pair<string, string> el : headers) {
        result += el.first + ": " + el.second + "\r\n";
    }
    return result;
}

Headers::Headers(unordered_map<string, string> headers) {
    this->headers = headers;
}

void Headers::setHeaders(unordered_map<string, string> headers) {
    this->headers = headers;
}

unordered_map<string, string> Headers::getHeaders() {
    return headers;
}

void Headers::add(const char *key, const char *value) {
    headers.insert({key, value});
}
