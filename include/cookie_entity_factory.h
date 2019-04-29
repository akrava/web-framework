#pragma once

#include <memory>
#include <unordered_map>
#include "entity.h"

/**
 * @brief Factory, that make clone of cookie entities prototypes
 *
 * Helper class, which store only one example of concrete type of cookie
 *      entity, and then make clone of this prototypes.
 */
class CookieEntityFactory {
public:
    /**
     * Enumeration of all popular cookie types
     */
    enum EntityType {
        Default,
        HttpOnly,
        WeekExpires
    };

    /**
     * Create factory and initialize all default CookieEntity types.
     */
    CookieEntityFactory();

    /**
     * Clone of existing cookie entity type with passed value
     *
     * @param entityType
     *      type from EntityType enumeration
     * @param value
     *      serialized value of cookie
     * @return
     *      new CookieEntity object
     */
    std::unique_ptr<Entity> createCookie(EntityType entityType, std::string & value);
private:
    std::unordered_map<EntityType, std::unique_ptr<Entity>> storage;
};
