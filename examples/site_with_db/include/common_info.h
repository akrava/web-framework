#pragma once

#include <akrava/web-server/handler.h>

class HandlerCommonInfo : public Handler {
    std::string key_in_db;
public:
    HandlerCommonInfo(const char * key_in_db, const char * ds, HTTP::Method m) : Handler(ds, m) {
        this->key_in_db = key_in_db;
    }
    const char * getKey() { return key_in_db.c_str(); }
    void exec() override;
};