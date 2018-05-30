#pragma once

#include <request.h>
#include <response.h>

class ParserHTTP {
public:
    static Request getRequestFromStr(std::string & str);
    static std::string getStrFromResponse(Response & response);
    static std::string urlEncode(const std::string & value);
    static std::string urlDecode(const std::string & value);
};


