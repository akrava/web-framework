#pragma once

#include <string>

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
	enum Version {
        HTTP_UNDEFINED,
	    HTTP_0_9,
        HTTP_1_0,
        HTTP_1_1,
        HTTP_2_0,
        HTTP_ANY
	};
    static HTTP::Method getMethod(std::string & str);
    static HTTP::Version getVersion(std::string & str);
    static std::string getVersion(HTTP::Version version);
    static std::string getReasonPhrase(int code);
};
