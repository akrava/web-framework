#pragma once

#include <sqlite3.h>
#include <akrava/web-server/middleware.h>
#include <vector>

class DatabaseMiddleware : public Middleware {
    DatabaseMiddleware * instance;
    sqlite3 *db;
    DatabaseMiddleware(const char *nameID, const char * filePath);
    ~DatabaseMiddleware();
public:
    static DatabaseMiddleware * getInstance(const char *nameID, const char * filePath);
    DatabaseMiddleware(DatabaseMiddleware const&) = delete;
    DatabaseMiddleware& operator= (DatabaseMiddleware const&) = delete;
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


    bool autoExec() {
        return false;
    }


    void exec() {

    }
};
