#pragma once

#include <string>
#include <unordered_map>


class Headers {
    std::unordered_map<std::string, std::string> headers;
public:
    Headers();
    Headers(std::string & httpHeaders);
    Headers(std::unordered_map<std::string, std::string> headers);
    std::string toString();
    std::unordered_map<std::string, std::string> getHeaders();
    void setHeaders(std::unordered_map<std::string, std::string> headers);
    void add(const char * key, const char * value);
    bool getValue(const char * key, std::string & value);
};

