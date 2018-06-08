#pragma once

#include <string>

/**
 * @brief wrapper class for http body
 *
 * MessageBody contains decoded information about http body
 */
class MessageBody {
    std::string body;
public:
    /**
     * Create empty http body
     */
    MessageBody();

    /**
     * Create http body from input string
     *
     * @param body
     *      input string
     */
    MessageBody(std::string & body);

    /**
     * set http body as string
     *
     * @param body
     *      http body
     */
    void setBody(std::string & body);

    /**
     * get http body as string
     *
     * @return
     *      http body
     */
    std::string getBody();
};
