#pragma once

#include <sqlite3.h>

class DBManager {
    sqlite3 *db;
    char *err;
public:
    DBManager(const char * filePath);
    ~DBManager();
    bool execQuery(const char * statement, std::vector<std::vector<std::string>> & result_vec, char * data[], int num);
};

