#pragma once

#include <string>
#include <unordered_map>

/**
 *
 */
class Headers {
    std::unordered_map<std::string, std::string> headers;
public:
    /**
     *
     */
    Headers();

    ~Headers();

    /**
     *
     * @param httpHeaders
     */
    Headers(std::string & httpHeaders);

    /**
     *
     * @return
     */
    std::string toString();

    /**
     *
     * @return
     */
    std::unordered_map<std::string, std::string> getHeaders();

    /**
     *
     * @param key
     * @param value
     */
    void add(const char * key, const char * value);

    /**
     *
     * @param key
     * @param value
     * @return
     */
    bool getValue(const char * key, std::string & value);
};
