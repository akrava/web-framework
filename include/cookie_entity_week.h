#pragma once

#include <string>
#include <ctime>
#include "entity.h"

/**
 * @brief Default CookieEntity which expires in a week since created
 *
 * Concrete prototype of cookie entity with all default fields except
 *      of expires feature, which is set as 7 days
 */
class CookieEntityWeek : public Entity {
    std::string value;
    std::time_t expires;
public:
    /**
     * Create default cookie entity  which expires in a week since created
     *
     * @param value
     *      serialized value
     */
    explicit CookieEntityWeek(const char * value);

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
     * Clone default cookie entity which expires in a week since created
     *
     * @return
     *      new CookieEntityWeek object
     */
    std::unique_ptr<Entity> clone() override;
};