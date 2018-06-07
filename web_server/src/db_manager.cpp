#include <string>
#include <vector>
#include "db_manager.h"

DBManager::DBManager(const char * filePath) {
    int status = sqlite3_open(filePath, &db);
    if (status) {
        err = (char *)sqlite3_errmsg(db);
    } else {
        err = nullptr;
    }
}

DBManager::~DBManager() {
    sqlite3_close(db);
}

bool DBManager::execQuery(const char * statement, std::vector<std::vector<std::string>> & result_vec, char * data[], int num) {
    sqlite3_stmt* stmt = nullptr;
    int status = sqlite3_prepare_v2(db, statement, -1, &stmt, nullptr);
    if (status != SQLITE_OK) return false;
    for (int i = 1; i <= num; i++) {
        if (sqlite3_bind_text(stmt, i, data[i-1], -1, nullptr) != SQLITE_OK) return false;
    }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::vector<std::string> row;
        for ( int colIndex = 0; colIndex < sqlite3_column_count( stmt ); colIndex++ ) {
            char * result = (char *)sqlite3_column_text( stmt, colIndex );
            row.emplace_back(result ? result : "");
        }
        result_vec.push_back(row);
    }
    if (sqlite3_clear_bindings(stmt) != SQLITE_OK) return false;
    if (sqlite3_reset(stmt) != SQLITE_OK) return false;
    return sqlite3_finalize(stmt) == SQLITE_OK;
}
