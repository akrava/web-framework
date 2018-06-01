#pragma once

#include <response.h>
#include <unordered_map>
#include "request.h"

class Middleware {
    std::string nameID;
protected:
    Request * request;
    Response * response;
    std::unordered_map<std::string, std::string> * map;
public:
    Middleware(const char * nameID, Request * request, Response * response);
    void setContent(Request * request, Response * response);
    void addValueToMap(const char * key, const char * value);
    bool getValueFromMap(const char * key, std::string & value);
    std::unordered_map<std::string, std::string> * getMap();
    virtual bool autoExec() = 0;
    virtual void exec() = 0;
    std::string getNameID();
};


