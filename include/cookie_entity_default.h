#pragma once

#include <string>
#include "entity.h"

/**
 * @brief Cookie entity with default params
 *
 * Concrete prototype of cookie entity with all default fields
 */
class CookieEntityDefault : public Entity {
    std::string value;
public:
    /**
     * Create default cookie entity
     *
     * @param value
     *      serialized value
     */
    explicit CookieEntityDefault(const char * value);

    /**
     * Set value of cookie
     *
     * @param value
     *      serialized data
     */
    void setValue(std::string value) override;

    /**
     * Serialize this cookie to string
     *
     * @return
     *      serialized cookie value
     */
    std::string toString() override;

    /**
     * Clone default cookie entity
     *
     * @return
     *      new CookieEntityDefault object
     */
    std::unique_ptr<Entity> clone() override;
};