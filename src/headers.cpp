#include "headers.h"

using namespace std;


Headers::Headers() {
    headers = unordered_map<string, string>();
}

Headers::Headers(string & httpHeaders) : Headers() {
    // TODO
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
