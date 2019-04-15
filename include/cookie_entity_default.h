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
     * @param value
     */
    void setValue(std::string value) override;

    /**
     *
     */
    std::string toString() override;

    /**
     *
     */
    std::unique_ptr<Entity> clone() override;
};