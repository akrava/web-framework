#pragma once

#include <string>
#include <ctime>

class CookieEntity {
    std::string value;
    std::time_t expires;
    size_t maxAge_sec;
    std::string domain;
    std::string path;
    bool httpOnly;
public:
    CookieEntity(const char * value, time_t expires = -1, size_t maxAge_sec = std::string::npos,
                 const char * domain = nullptr, const char *  path = nullptr, bool httpOnly = false);
    std::string toString();
};
