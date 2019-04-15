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
     * @param value
     */
    void setValue(std::string value) override;

    /**
     *
     */
    std::unique_ptr<Entity> clone() override;
};