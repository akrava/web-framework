#include "user.h"
#include <memory>
#include "database_middleware.h"
#include "config.h"
#include <akrava/web-server/parser_http.h>

using namespace std;

std::unique_ptr<Entity> User::clone() {
    return std::make_unique<User>(*this);
}

std::string User::toString() {
    return uuid;
}

void User::setValue(std::string value) {
    uuid = value;
}

User::User(int id, std::string &uuid, std::string &name, std::string &email, std::string &passHashed) {
    this->id = id;
    this->uuid = uuid;
    this->name = name;
    this->email = email;
    this->passHashed = passHashed;
}

std::string User::serialize(Entity * entity) {
    return entity->toString();
}

Entity *User::deserialize(std::string & value) {
    auto db = DatabaseMiddleware::getInstance("db", __PATH_TO_DB);

    char * data[] = { (char *)value.c_str() };
    vector<vector<string>> result_user;
    if (!db->execQuery(
            "SELECT * FROM users WHERE uuid = ?",
            result_user,
            data,
            1)
    ) return nullptr;

    if (result_user.size() != 1 || result_user[0].size() != 5) {
        return nullptr;
    }
    return new User(
        std::stoi(result_user[0][0]), result_user[0][1], result_user[0][2],
        result_user[0][3], result_user[0][4]
    );
}

Entity * User::checkLogin(std::string &username, std::string &value) {
    auto db = DatabaseMiddleware::getInstance("db", __PATH_TO_DB);

    char * data[] = { (char *)username.c_str() };
    vector<vector<string>> result_user{};
    if (!db->execQuery(
            "SELECT * FROM users WHERE email = ?",
            result_user,
            data,
            1)
    ) return nullptr;

    if (result_user.size() != 1 || result_user[0].size() != 5) {
        return nullptr;
    }
    string secret = __SECRET;
    if (result_user[0][4] != ParserHTTP::sha256(secret, value)) {
        return nullptr;
    }
    return new User(
            std::stoi(result_user[0][0]), result_user[0][1], result_user[0][2],
            result_user[0][3], result_user[0][4]
    );
}

std::string User::getName() {
    return name;
}
