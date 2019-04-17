#include "parser_http.h"
#include "form_urlencoded.h"

using namespace std;

void FormUrlEncoded::parseNextKey(size_t startPosition, size_t endPosition) {
    string key = formBody.substr(startPosition, endPosition - startPosition);
    keyCurrent = ParserHTTP::urlDecode(key);
}

void FormUrlEncoded::parseNextValue(size_t startPosition, size_t endPosition) {
    string value = formBody.substr(startPosition, endPosition - startPosition);
    value = ParserHTTP::urlDecode(value);
    valueCurrent = new FormEntity(value);
}

FormUrlEncoded::FormUrlEncoded() {
    pairsDelimiter = "&";
    valuesDelimiter = "=";
}
