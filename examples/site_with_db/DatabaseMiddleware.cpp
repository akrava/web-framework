#include <pqxx/result>
#include "DatabaseMiddleware.h"

using namespace std;

static pqxx::prepare::invocation& prep_dynamic(char **data, int num, pqxx::prepare::invocation& inv);

DatabaseMiddleware::DatabaseMiddleware(const char *nameID, const char * connectionString) 
	: Middleware(nameID), connection(connectionString), action(connection) { }

DatabaseMiddleware::~DatabaseMiddleware() {
	connection.disconnect();
}

bool DatabaseMiddleware::execQuery(const char *statement, std::vector<std::vector<std::string>> &result_vec, char **data, int num) {
	connection.prepare("query", statement);
	auto invocation = action.prepared("query");
	prep_dynamic(data, num, invocation);
	auto result = invocation.exec();
	action.commit();

	for (auto i = result.begin(), r_end = result.end(); i != r_end; ++i) {
		vector<string> row;
		for (auto f = i->begin(), f_end = i->end(); f != f_end; ++f) {
			row.emplace_back(!f.is_null() ? f->c_str() : "");
		}
		result_vec.push_back(row);
	}
    return true;
}

//Just give a vector of data you can change the template<int> to any data type
static pqxx::prepare::invocation& prep_dynamic(char **data, int num, pqxx::prepare::invocation& inv) {
	for (int i = 1; i <= num; i++) {
		inv(data[i - 1]);
	}
	return inv;
}


