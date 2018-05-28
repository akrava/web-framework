#pragma once
#include <string>

class MessageBody {
    std::string body;
public:
    MessageBody();
    MessageBody(std::string & body);
    void setBody(std::string & body);
    std::string getBody();
};
