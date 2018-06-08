#pragma once

#include <request.h>
#include <response.h>

/**
 *
 */
class ParserHTTP {
public:
    /**
     *
     * @param str
     * @return
     */
    static Request * getRequestFromStr(std::string & str);

    /**
     *
     * @param response
     * @return
     */
    static std::string getStrFromResponse(Response & response);

    /**
     *
     * @param value
     * @return
     */
    static std::string urlEncode(const std::string & value);

    /**
     *
     * @param value
     * @return
     */
    static std::string urlDecode(const std::string & value);

    /**
     *
     * @param tm
     * @param format
     * @return
     */
    static std::string getTime(const time_t * time_struct = nullptr, const char * format = "%Y.%m.%d");
};
