#include <cstring>
#include "jwt_auth.h"
#include "runtime_exception.h"
#include "parser_http.h"
#include "json_middleware.h"

using namespace std;

JwtAuth::JwtAuth(const char * secret, Middleware * json) {
    this->secret = secret;
    middleware = json;
    headersToken = R"({ "alg": "HS256", "typ": "JWT" })";
}

bool JwtAuth::checkHeadersToParse(Headers * headers) {
    if (!headers) {
        throw RuntimeException("Headers was not set");
    }
    std::string value;
    return headers->getValue("Authorization", value) && value.find("Bearer ") == 0;
}

std::string JwtAuth::getSerializedValue(Headers * headers) {
    if (!headers) {
        throw RuntimeException("Headers was not set");
    }
    std::string value;
    if (!headers->getValue("Authorization", value) && value.find("Bearer ") == 0) {
        return "";
    }
    value.erase(0, strlen("Bearer "));
    size_t firstDot = value.find('.');
    if (firstDot == string::npos) {
        return "";
    }
    string tokenHeaders = value;
    tokenHeaders.substr(0, firstDot);

    size_t secondDot = value.find('.', firstDot);
    if (secondDot == string::npos) {
        return "";
    }
    string bodyToken = value;
    bodyToken.substr(firstDot, secondDot);

    string signToken = value;
    signToken.substr(secondDot);

    if (ParserHTTP::base64Encode(tokenHeaders) != headersToken) {
        return "";
    }

    string realSign = tokenHeaders + "." + bodyToken;
    if (signToken != ParserHTTP::sha256(secret, realSign)) {
        return "";
    }
    return ParserHTTP::base64Decode(bodyToken);
}

void JwtAuth::serializeAndSetValueToResponse(std::string & value) {
    string headers = ParserHTTP::base64Encode(headersToken);
    string body = ParserHTTP::base64Encode(value);
    string encoded = headers + "." + body;
    string sign = ParserHTTP::sha256(secret, encoded);
    string token = headers + '.' + body + '.' + sign;
    if (!middleware)  {
        throw RuntimeException("Middleware was not set");
    }
    auto json = (JsonMiddleware *) middleware;
    json->addValueToMap("token", token.c_str());
    json->fillResponse();
}
