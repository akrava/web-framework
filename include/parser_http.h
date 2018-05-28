#pragma once

#include <request.h>
#include <response.h>

class ParserHTTP {
public:
    static Request getRequestFromStr(std::string & str);
    static std::string getStrFromResponse(Response & response);
};


