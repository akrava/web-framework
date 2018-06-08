#pragma once

#include <sqlite3.h>

/**
 *
 */
class DBManager {
    sqlite3 *db;
    char *err;
public:
    /**
     *
     * @param filePath
     */
    DBManager(const char * filePath);
    ~DBManager();

    /**
     *
     * @param statement
     * @param result_vec
     * @param data
     * @param num
     * @return
     */
    bool execQuery(const char * statement, std::vector<std::vector<std::string>> & result_vec, char * data[], int num);
};
