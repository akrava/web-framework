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
     */
    std::unique_ptr<Entity> clone() override;
};