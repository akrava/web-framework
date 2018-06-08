#pragma once

#include <string>
#include <unordered_map>

/**
 *
 */
class URI {
    std::string raw_data;
    std::string uri;
    std::unordered_map<std::string, std::string> params;
public:
    /**
     *
     */
    URI();

    /**
     *
     * @param uri
     */
    URI(std::string & uri);

    /**
     *
     * @param key
     * @param value
     * @return
     */
    bool getValueFromParam(const char * key, std::string & value);

    /**
     *
     * @param uri
     */
    void setParamsAndUri(std::string & uri);

    /**
     *
     * @return
     */
    std::string getUri();

    std::string getRawData();

    /**
     *
     * @return
     */
    std::unordered_map<std::string, std::string> getParams();
};
