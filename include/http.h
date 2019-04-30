#pragma once

#include <string>


/**
 * @brief static class describes http method, version, and allow to convert it from/to string/enumeration
 *
 * HTTP class describes Method, Version, ReasonPhrase of code in http
 */
class HTTP {
public:
	/**
	* Flags to define combinations of HTTP Request methods
	*/
	enum Method {
	    UNDEFINED,
		GET,
		HEAD,
		POST ,
		PUT,
		DELETE,
		CONNECT,
		OPTIONS,
		TRACE,
		PATCH,
        ANY
	};

	/**
	 * Flags to define combinations of HTTP Version
	 */
	enum Version {
        HTTP_UNDEFINED,
	    HTTP_0_9,
        HTTP_1_0,
        HTTP_1_1,
        HTTP_2_0,
        HTTP_ANY
	};

	/**
	 * Parse input string to http method
	 *
	 * @param str
	 * 		input string
	 * @return
	 * 		parsed method from string, if string wasn't valid returns UNDEFINED
	 */
    static HTTP::Method getMethod(std::string & str);

    /**
     * Parse input string to http version
     *
     * @param str
     * 		input string
     * @return
     * 		parsed version from string, if string wasn't valid returns HTTP_UNDEFINED
     */
    static HTTP::Version getVersion(std::string & str);

    /**
     * Serialize HTTP::Version to string
     *
     * @param version
     * 		http version
     * @return
     * 		version as string
     */
    static std::string getVersion(HTTP::Version version);

    /**
     * Serialize status code to string
     *
     * @param code
     * 		http status code
     * @return
     * 		reason phrase for code as string, returns Not Found if code not found among values
     */
    static std::string getReasonPhrase(int code);

    /**
     * Get mime-type of file by its extension
     *
     * @param extension
     *      extension of file without a dot
     * @return
     *      mime-type of current file
     */
    static std::string getMimeType(std::string & extension);
};
