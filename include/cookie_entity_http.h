#pragma once

#include <string>
#include "entity.h"

/**
 * @brief Default CookieEntity with http only feature
 *
 * Concrete prototype of cookie entity with all default fields and
 *      http only property
 */
class CookieEntityHttp : public Entity {
    std::string value;
    bool httpOnly;
public:
    /**
     * Create default cookie entity with http only feature
     *
     * @param value
     *      serialized value
     */
    explicit CookieEntityHttp(const char * value);

    /**
     * Serialize this cookie to string
     *
     * @return
     *      serialized cookie value
     */
    std::string toString() override;

    /**
     * Set value of cookie
     *
     * @param value
     *      serialized data
     */
    void setValue(std::string value) override;

    /**
     * Clone default cookie entity with http only feature
     *
     * @return
     *      new CookieEntityHttp object
     */
    std::unique_ptr<Entity> clone() override;
};