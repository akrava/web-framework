#pragma once

#include <string>
#include <entity.h>
#include <ctime>

/**
 *
 */
class CookieEntityWeek : public Entity {
    std::string value;
    std::time_t expires;
public:
    /**
     *
     */
    CookieEntityWeek(const char * value);

    /**
     *
     */
    std::string toString() override;

    /**
     *
     */
    std::unique_ptr<Entity> clone() override;
};