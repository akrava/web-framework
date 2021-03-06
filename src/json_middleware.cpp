#include "json_middleware.h"

JsonMiddleware::JsonMiddleware(const char * nameID) : Middleware (nameID) {
  jsonRequest = new nlohmann::json();
  jsonResponse = new nlohmann::json();
  errorDeserialize = false;
}

bool JsonMiddleware::autoExec() {
    jsonResponse->clear();
    jsonRequest->clear();
    std::string value;
    if (request->getHeaders()->getValue("Content-Type", value)) {
        return value.find("application/json") == 0;
    } else {
        return false;
    }
}

void JsonMiddleware::exec() {
    delete jsonRequest;
    jsonRequest = new nlohmann::json(nlohmann::json::parse(
            request->getMessageBody()->getBody(),
            nullptr,
            false)
    );
    if (jsonRequest->is_discarded()) {
        errorDeserialize = true;
    }
}


void JsonMiddleware::fillResponse() {
    response->getHeaders()->add("Content-Type", "application/json");
    std::string res = jsonResponse->dump(4, ' ');
    response->getBody()->setBody(res);
}

nlohmann::json * JsonMiddleware::getJsonRequest() {
    return jsonRequest;
}

nlohmann::json * JsonMiddleware::getJsonResponse() {
    return jsonResponse;
}

JsonMiddleware::~JsonMiddleware() {
    delete jsonResponse;
    delete jsonRequest;
    jsonRequest = nullptr;
    jsonResponse = nullptr;
}

void JsonMiddleware::clear() {
    Middleware::clear();
    if (jsonRequest) jsonRequest->clear();
    if (jsonResponse) jsonResponse->clear();
    errorDeserialize = false;
}
