#include <http.h>
#include <string>

using namespace std;

HTTP::Method HTTP::getMethod(std::string & str) {
    if (str == "GET") {
        return HTTP::Method::GET;
    } else if (str == "HEAD") {
        return HTTP::Method::HEAD;
    } else if (str == "POST") {
        return HTTP::Method::POST;
    } else if (str == "PUT") {
        return HTTP::Method::PUT;
    } else if (str == "DELETE") {
        return HTTP::Method::DELETE;
    } else if (str == "CONNECT") {
        return HTTP::Method::CONNECT;
    } else if (str == "OPTIONS") {
        return HTTP::Method::OPTIONS;
    } else if (str == "TRACE") {
        return HTTP::Method::TRACE;
    } else if (str == "PATCH") {
        return HTTP::Method::PATCH;
    } else {
        return HTTP::Method::UNDEFINED;
    }
}

HTTP::Version HTTP::getVersion(std::string & str) {
    if (str == "HTTP/0.9") {
        return HTTP::Version::HTTP_0_9;
    } else if (str == "HTTP/1.0") {
        return HTTP::Version::HTTP_1_0;
    } else if (str == "HTTP/1.1") {
        return HTTP::Version::HTTP_1_1;
    } else if (str == "HTTP/2.0") {
        return HTTP::Version::HTTP_2_0;
    } else {
        return HTTP::Version::HTTP_UNDEFINED;
    }
}

std::string HTTP::getVersion(HTTP::Version version) {
    switch (version) {
        case HTTP::Version::HTTP_0_9 : return "HTTP/0.9";
        case HTTP::Version::HTTP_1_0 : return "HTTP/1.0";
        case HTTP::Version::HTTP_1_1 : return "HTTP/1.1";
        case HTTP::Version::HTTP_2_0 : return "HTTP/2.0";
        default: return "HTTP/1.1";
    }
}

std::string HTTP::getReasonPhrase(int code) {
    switch (code) {
        case 100: return "Continue";
        case 101: return "Switching Protocols";
        case 200: return "OK";
        case 201: return "Created";
        case 202: return "Accepted";
        case 203: return "Non-Authoritative Information";
        case 204: return "No Content";
        case 205: return "Reset Content";
        case 206: return "Partial Content";
        case 300: return "Multiple Choices";
        case 301: return "Moved Permanently";
        case 302: return "Moved Temporarily";
        case 303: return "See Other";
        case 304: return "Not Modified";
        case 305: return "Use Proxy";
        case 400: return "Bad Request";
        case 401: return "Unauthorized";
        case 402: return "Payment Required";
        case 403: return "Forbidden";
        case 404: return "Not Found";
        case 405: return "Method Not Allowed";
        case 406: return "Not Acceptable";
        case 407: return "Proxy Authentication Required";
        case 408: return "Request Time-out";
        case 409: return "Conflict";
        case 410: return "Gone";
        case 411: return "Length Required";
        case 412: return "Precondition Failed";
        case 413: return "Request Entity Too Large";
        case 414: return "Request-URI Too Large";
        case 415: return "Unsupported Media Type";
        case 500: return "Internal Server Error";
        case 501: return "Not Implemented";
        case 502: return "Bad Gateway";
        case 503: return "Service Unavailable";
        case 504: return "Gateway Time-out";
        case 505: return "HTTP Version not supported";
        default: return "Not Found";
    }
}
