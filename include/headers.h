#pragma once

#include <string>
#include <unordered_map>

/**
 * @brief wrapper class for http headers
 *
 * Headers consist of map with key-value pairs, and is using for Request and Response http objects
 */
class Headers {
    std::unordered_map<std::string, std::string> headers;
public:
    /**
     * create empty headers object
     */
    Headers();

    /**
     * cleanup map of key-value pairs
     */
    ~Headers();

    /**
     * create Headers, parsing http input string
     *
     * @param httpHeaders
     *      input http headers string
     */
    Headers(std::string & httpHeaders);

    /**
     * serialize Headers to string
     *
     * @return
     *      serialized string
     */
    std::string toString();

    /**
     * get current map
     *
     * @return
     *      map of key-value pairs
     */
    std::unordered_map<std::string, std::string> getHeaders();

    /**
     * insert value by key to map, if key exists, it will be overwritten
     *
     * @param key
     *      input key
     * @param value
     *      input value
     */
    void add(const char * key, const char * value);

    /**
     * get value from map by key
     *
     * @param key
     *      searched key
     * @param value
     *      out param, if key exists, value will be written, nothing do otherwise
     * @return
     *      true if value exists, false otherwise
     */
    bool getValue(const char * key, std::string & value);

    /**
     * delete all data
     */
    void clear();
};
