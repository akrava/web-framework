#include "parser_http.h"
#include "form_middleware.h"
#include "form_urlencoded.h"
#include "form_multipart_data.h"

using namespace std;

bool FormMiddleware::autoExec() {
    std::string value;
    if (request->getHeaders()->getValue("Content-Type", value)) {
        auto urlencoded = value.find("application/x-www-form-urlencoded") == 0;
        auto multipart = value.find("multipart/form-data") == 0;
        if (urlencoded) {
            setParser(new FormUrlEncoded());
        }
        if (multipart) {
            setParser(new FormMultipart());
        }
        return urlencoded || multipart;
    } else {
        return false;
    }
}

void FormMiddleware::exec() {
    string data = request->getRawData();
    delete keysStorage;
    keysStorage = currentParser->parseForm(data, map);
}

FormMiddleware::~FormMiddleware() {
    delete currentParser;
}

void FormMiddleware::setParser(FormParser *parser) {
    delete currentParser;
    currentParser = parser;

}

FormParser *FormMiddleware::getParser() {
    return currentParser;
}

FormEntity *FormMiddleware::getFormEntity(const char * key) {
    if (keysStorage->at(key)) {
        return keysStorage->at(key);
    }
    return nullptr;
}
