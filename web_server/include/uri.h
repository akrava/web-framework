#pragma once

#include <string>
#include <unordered_map>

/**
 * @brief class represents http uri
 *
 * URI consist of uri - a string of uri without arguments, and map of key-value pairs
 *      which are deserialized parameters of uri
 */
class URI {
    std::string raw_data;
    std::string uri;
    std::unordered_map<std::string, std::string> params;
public:
    /**
     * Create empty URI object
     */
    URI();

    /**
     * Construct URI object from string, deserialize all params and decode input string
     *
     * @param uri
     *      http uri string
     */
    URI(std::string & uri);

    /**
     * get value by key from uri
     *
     * @param key
     *      key in params of http uri
     * @param value
     *      out param, where will be written value if exists or do nothing otherwise
     * @return
     *      true if key exists in map, false otherwise
     */
    bool getValueFromParam(const char * key, std::string & value);

    /**
     * deserialize and decode input http uri into path and params
     *
     * @param uri
     *      http uri string
     */
    void setParamsAndUri(std::string & uri);

    /**
     * get decoded uri path without params
     *
     * @return
     *      http uri string without params
     */
    std::string getPath();

    /**
     * get unchanged uri string with params
     *
     * @return
     *      http uri string
     */
    std::string getRawData();

    /**
     * get deserialized and decoded map of http uri params
     *
     * @return
     *      map of key-value pairs from uri params
     */
    std::unordered_map<std::string, std::string> getParams();
};
