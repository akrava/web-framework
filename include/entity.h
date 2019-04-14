#pragma once

#include <memory>

/**
 *
 */
class Entity {
public:
    /**
     *
     */
    virtual ~Entity() = default;

    /**
     *
     * @return
     */
    virtual std::string toString() = 0;

    /**
     *
     * @return
     */
    virtual std::unique_ptr<Entity> clone() = 0;
};