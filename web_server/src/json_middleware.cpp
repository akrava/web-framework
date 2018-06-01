#include "json_middleware.h"

JsonMiddleware::JsonMiddleware(const char * nameID, Request * request, Response * response) : Middleware (nameID, request, response) {
  jsonRequest = nlohmann::json();
  jsonResponse = nlohmann::json();
  errorDeserialize = false;
}

bool JsonMiddleware::autoExec() {
    std::string value;
    if (request->getHeaders()->getValue("Content-Type", value)) {
        return value.find("application/json") == 0;
    } else {
        return false;
    }
}

void JsonMiddleware::exec() {
    jsonRequest = nlohmann::json::parse(request->getMessageBody()->getBody(), nullptr, false);
    if (jsonRequest.is_discarded()) {
        errorDeserialize = true;
    }
}

void JsonMiddleware::setEchoReply() {
    jsonResponse = {
            {"data", jsonRequest.is_discarded() ? nlohmann::json(nullptr) : jsonRequest},
            {"Echo mode enabled", true},
            {"middleware", "JSON"}
    };
}


void JsonMiddleware::fillResponse() {
    response->getHeaders()->add("Content-Type", "application/json");
    std::string res = jsonResponse.dump();
    response->getBody()->setBody(res);
}
