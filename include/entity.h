#pragma once

#include <memory>

/**
 * @brief Cloneable interface
 */
class Entity {
public:
    /**
     *
     */
    virtual ~Entity() = default;

    /**
     *
     * @param value
     */
    virtual void setValue(std::string value) = 0;

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