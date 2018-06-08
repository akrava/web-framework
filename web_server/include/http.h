#pragma once

#include <string>


/**
 *
 */
class HTTP {
public:
	/**
	*   Flags to define combinations of HTTP Request methods
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
	 *
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
	 *
	 * @param str
	 * @return
	 */
    static HTTP::Method getMethod(std::string & str);

    /**
     *
     * @param str
     * @return
     */
    static HTTP::Version getVersion(std::string & str);

    /**
     *
     * @param version
     * @return
     */
    static std::string getVersion(HTTP::Version version);

    /**
     *
     * @param code
     * @return
     */
    static std::string getReasonPhrase(int code);
};
