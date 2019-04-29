#pragma once

#include <memory>

/**
 * @brief Cloneable interface of object wth value and serializable method
 *
 * Abstract class with value and some methods. Could be cloned by method clone
 */
class Entity {
public:
    /**
     * default destructor
     */
    virtual ~Entity() = default;

    /**
     * Set serialized value
     *
     * @param value
     *      serialized value
     */
    virtual void setValue(std::string value) = 0;

    /**
     * Serialize current entity
     *
     * @return
     *      get string representation
     */
    virtual std::string toString() = 0;

    /**
     * Clone this object
     *
     * @return
     *      new Entity object
     */
    virtual std::unique_ptr<Entity> clone() = 0;
};