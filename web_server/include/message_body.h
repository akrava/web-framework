#pragma once

#include <string>

/**
 *
 */
class MessageBody {
    std::string body;
public:
    /**
     *
     */
    MessageBody();

    /**
     *
     * @param body
     */
    MessageBody(std::string & body);

    /**
     *
     * @param body
     */
    void setBody(std::string & body);

    /**
     *
     * @return
     */
    std::string getBody();
};
