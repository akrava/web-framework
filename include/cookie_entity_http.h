#pragma once

#include <string>
#include <entity.h>

/**
 *
 */
class CookieEntityHttp : public Entity {
    std::string value;
    bool httpOnly;
public:
    /**
     *
     */
    CookieEntityHttp(const char * value);

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