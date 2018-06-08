#pragma once

#include <sqlite3.h>

/**
 * @brief allow perform sql queries to db
 *
 * Realisation of wrapper for SQLite database
 */
class DBManager {
    sqlite3 *db;
public:
    /**
     * Opening database
     *
     * @param filePath
     *      file path to database
     */
    DBManager(const char * filePath);

    /**
     * Closing opened connection to db
     */
    ~DBManager();

    /**
     * Execute SQL statement, with binding values by escaping with '?' in statement
     *      and including them in data array
     *
     * @param statement
     *      SQL query
     * @param result_vec
     *      out parameter, used to write result table of executed query
     * @param data
     *      some data can be missed by ? in statement. So it's providing in this data array
     * @param num
     *      number of binding (preparing) values
     * @return
     *      true if executed successfully, false otherwise
     */
    bool execQuery(const char * statement, std::vector<std::vector<std::string>> & result_vec,
                   char * data[], int num);
};
