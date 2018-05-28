#include <request.h>
#include <response.h>
#include <parser_http.h>
#include "parser_http.h"
#include <http.h>

using namespace std;

Request ParserHTTP::getRequestFromStr(string & str) {
    //
    size_t methodEndPos = str.find_first_of(' ');
    if (methodEndPos == string::npos) return Request();
    string methodStr = str.substr(0, methodEndPos - 1);
    HTTP::Method method = HTTP::getMethod(methodStr);
    //
    size_t uriEndPos = str.find_first_of(' ', methodEndPos + 1);
    if (uriEndPos == string::npos) return Request();
    string uriStr = str.substr(methodEndPos + 1, uriEndPos - methodEndPos - 1);
    //
    size_t versionEndPos = str.find_first_of("\r\n", uriEndPos + 1);
    if (versionEndPos == string::npos) return Request();
    string versionStr = str.substr(uriEndPos + 1, versionEndPos - uriEndPos - 1);
    HTTP::Version version = HTTP::getVersion(versionStr);
    //
    size_t headersEndPos = str.find_first_of("\r\n\r\n", versionEndPos + 2);
    if (headersEndPos == string::npos) return Request();
    string headersStr = str.substr(versionEndPos + 2, headersEndPos - versionEndPos - 2);
    //
    string bodyStr = str.substr(headersEndPos + 4);
    return Request(method, uriStr, version, headersStr, bodyStr);
}

string ParserHTTP::getStrFromResponse(Response & response) {

}