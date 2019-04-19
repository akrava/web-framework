#pragma once

#include <string>
#include <akrava/web-server/entity.h>

class User : public Entity {
    int id;
    std::string uuid;
    std::string name;
    std::string email;
    std::string passHashed;
public:
    User(int id, std::string & uuid, std::string & name, std::string & email, std::string & passHashed);

    void setValue(std::string value) override;

    std::string toString() override;

    std::unique_ptr<Entity> clone() override;

    static std::string serialize(Entity * entity);

    static Entity * deserialize(std::string & value);

    static Entity * checkLogin(std::string & username, std::string & value);

    std::string getName();
};