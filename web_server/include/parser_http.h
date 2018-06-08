#pragma once

#include <request.h>
#include <response.h>

/**
 * @brief static class for parsing, encoding, decoding any http data
 *
 * ParserHTTP is used to serialize and deserialize http request, response etc.
 */
class ParserHTTP {
public:
    /**
     * Deserialize http request from input string
     *
     * @param str
     *      input string
     * @return
     *      deserialized Request object
     */
    static Request * getRequestFromStr(std::string & str);

    /**
     * Serialize http response into string
     *
     * @param response
     *      Response object
     * @return
     *      serialized string
     */
    static std::string getStrFromResponse(Response & response);

    /**
     * Encode input string
     *
     * @param value
     *      input string
     * @return
     *      encoded string
     */
    static std::string urlEncode(const std::string & value);

    /**
     * Decode input string
     *
     * @param value
     *      input string
     * @return
     *      decoded string
     */
    static std::string urlDecode(const std::string & value);

    /**
     * get date stamp in string in format from time_t
     *
     * @param time_struct
     *      required time in time_t, if nullptr - execute current time
     * @param format
     *      format of representing date in string
     * @return
     *      date stamp as string
     */
    static std::string getTime(const time_t * time_struct = nullptr, const char * format = "%Y.%m.%d");
};
