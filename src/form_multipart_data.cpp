#include <cstring>
#include "form_multipart_data.h"
#include "runtime_exception.h"
#include "parser_http.h"
#include "form_entity.h"

using namespace std;

static string getValue(string & source, const char * pre, const char * after);

void FormMultipart::actionBeforeSelectingBody() {
    string contentType = getValue(formBody, "Content-Type:", "\n");
    string boundary = getValue(contentType, "boundary=", "\n");
    pairsDelimiter = "--" + ParserHTTP::trim(boundary);
    string lastDelimiter = pairsDelimiter + "--";
    size_t lastDelimiterPos = formBody.find(lastDelimiter);
    if (lastDelimiterPos == string::npos) {
        throw RuntimeException("Couldn't parse multipart/form-data");
    }
    formBody.erase(lastDelimiterPos, lastDelimiter.length());
}

void FormMultipart::parseNextKey(size_t startPosition, size_t endPosition) {
    string current = formBody.substr(startPosition, endPosition - startPosition);
    string key = getValue(current, "name=\"", "\"");
    keyCurrent = key;
}

void FormMultipart::parseNextValue(size_t startPosition, size_t endPosition) {
    string current = formBody.substr(startPosition, endPosition - startPosition);
    current = ParserHTTP::trim(current);
    valueCurrent->setValue(current);
}

void FormMultipart::actionBeforeValueParsing(size_t keyStart, size_t keyEnd) {
    string current = formBody.substr(keyStart, keyEnd - keyStart);
    valueCurrent = new FormEntity(current);
    if (current.find("filename=\"") != string::npos) {
        string fileName = getValue(current, "filename=\"", "\"");
        valueCurrent->setFileName(fileName);
    }
    if (current.find("Content-Type:") != string::npos) {
        string contentType = getValue(current, "Content-Type:", "\n");
        contentType = ParserHTTP::trim(contentType);
        valueCurrent->setContentType(contentType);
    }
    if (current.find("Content-Type:") != string::npos) {
        string contentType = getValue(current, "Content-Type:", "\n");
        contentType = ParserHTTP::trim(contentType);
        valueCurrent->setContentType(contentType);
    }
}

FormMultipart::FormMultipart() {
    valuesDelimiter = '\n';
}

string getValue(string & source, const char * pre, const char * after) {
    size_t startPos = source.find(pre);
    int prefixLength = strlen(pre);
    if (startPos == string::npos || startPos + prefixLength <= source.length()) {
        throw RuntimeException("Couldn't parse multipart/form-data");
    }
    size_t endPos = source.find(after, startPos + prefixLength);
    if (endPos == string::npos) {
        return source.substr(startPos + prefixLength);
    }
    return source.substr(startPos + prefixLength, endPos - prefixLength - startPos);
}