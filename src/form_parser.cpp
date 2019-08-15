#include "form_parser.h"
#include "runtime_exception.h"

using namespace std;

KeyEntityStorage * FormParser::parseForm(std::string & httpRequest, KeyValueStorage *KVPStorage) {
    formBody = httpRequest;
    actionBeforeSelectingBody();
    selectFormBody();
    size_t position = 0;
    while (position < formBody.length()) {
        valueCurrent = nullptr;
        size_t endKeyPos = formBody.find(valuesDelimiter, position);
        if (endKeyPos == string::npos || endKeyPos <= position) {
            break;
        }
        parseNextKey(position, endKeyPos);
        actionBeforeValueParsing(position, endKeyPos);
        position = endKeyPos + valuesDelimiter.length();
        size_t endValuePos = formBody.find(pairsDelimiter, position);
        if (endValuePos == string::npos || endValuePos <= position) {
            endValuePos = formBody.length();
        }
        parseNextValue(position, endValuePos);
        values->insert({keyCurrent, valueCurrent});
        position = endValuePos + pairsDelimiter.length();
    }
    if (KVPStorage) {
        *KVPStorage = getKeyValuePairs();
    }
    return values;
}

void FormParser::selectFormBody() {
    string source = formBody;
    size_t headersEndPos = source.find("\r\n\r\n");
    if (headersEndPos == string::npos || headersEndPos + 4 >= source.length()) {
        throw RuntimeException("Request body is invalid");
    }
    formBody = source.substr(headersEndPos + 4);
}

KeyValueStorage FormParser::getKeyValuePairs() {
    KeyValueStorage result;
    for (auto & cur : *values) {
        result.insert({cur.first, cur.second->getValue()});
    }
    return result;
}

void FormParser::setCurrentValue(FormEntity * value) {
    delete this->valueCurrent;
    this->valueCurrent = value;
}

FormParser::~FormParser() {
    delete valueCurrent;
}