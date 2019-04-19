#pragma once

#include <memory>
#include <unordered_map>
#include "entity.h"

/**
 * @brief Clone prototypes of cookie entities
 */
class CookieEntityFactory {
public:
    /**
     *
     */
    enum EntityType {
        Default,
        HttpOnly,
        WeekExpires
    };

    /**
     *
     */
    CookieEntityFactory();

    /**
     *
     * @param entityType
     * @return
     */
    std::unique_ptr<Entity> createCookie(EntityType entityType, std::string & value);
private:
    std::unordered_map<EntityType, std::unique_ptr<Entity>> storage;
};
