#pragma once
#include <string>
#include <unordered_map>


class URI {
    std::string raw_data;
    std::string uri;
    std::unordered_map<std::string, std::string> params;
public:
    URI();
    URI(std::string & uri);
    void setRawData(std::string & uri);
    std::string getUri();
    std::unordered_map<std::string, std::string> getParams();
};
