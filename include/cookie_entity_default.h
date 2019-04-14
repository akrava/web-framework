#pragma once

#include <string>
#include <entity.h>

/**
 *
 */
class CookieEntityDefault : public Entity {
    std::string value;
public:
    /**
     *
     */
    CookieEntityDefault(const char * value);

    /**
     *
     */
    std::string toString() override;

    /**
     *
     */
    std::unique_ptr<Entity> clone() override;
};