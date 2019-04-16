#include <http.h>

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
        case 302: return "Found";
        case 303: return "See Other";
        case 304: return "Not Modified";
        case 305: return "Use Proxy";
        case 307: return "Temporary Redirect";
        case 308: return "Permanent Redirect";
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

string HTTP::getMimeType(string & extension) { 
    if (extension == "aac") {
        return "audio/aac";
    } else if (extension == "abw") {
        return "application/x-abiword";
    } else if (extension == "arc") {
        return "application/x-freearc";
    } else if (extension == "avi") {	
        return "video/x-msvideo";
    } else if (extension == "azw") {	
        return "application/vnd.amazon.ebook";
    } else if (extension == "bin") {
        return "application/octet-stream";
    } else if (extension == "bmp") {  
        return "image/bmp";
    } else if (extension == "bz") {	
        return "application/x-bzip";
    } else if (extension == "bz2") {	
        return "application/x-bzip2";
    } else if (extension == "csh") {	
        return "application/x-csh";
    } else if (extension == "css") {	
        return "text/css";
    } else if (extension == "csv") {	
        return "text/csv";
    } else if (extension == "doc") {	
        return "application/msword";
    } else if (extension == "docx") {
        return "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
    } else if (extension == "eot") {	
        return "application/vnd.ms-fontobject";
    } else if (extension == "epub") {
        return "application/epub+zip";
    } else if (extension == "gif") {
        return "image/gif";
    } else if (extension == "htm") { 
        return "text/html";
    } else if (extension == "html")	{
        return "text/html";
    } else if (extension == "ico") {   
        return "image/vnd.microsoft.icon";
    } else if (extension == "ics") {	 
        return "text/calendar";
    } else if (extension == "jar") {	 
        return "application/java-archive";
    } else if (extension == "jpeg") { 
        return "image/jpeg";
    } else if (extension == "jpg") {	 
        return "image/jpeg";
    } else if (extension == "js") {	 
        return "text/javascript";
    } else if (extension == "json") { 
        return "application/json";
    } else if (extension == "jsonld") { 	
        return "application/ld+json";
    } else if (extension == "mid") {      
        return "audio/midi";
    } else if (extension == "midi") { 	
        return "audio/midi";
    } else if (extension == "mjs") { 	
        return "text/javascript";
    } else if (extension == "mp3") {	
        return "audio/mpeg";
    } else if (extension == "mpeg") {	
        return "video/mpeg";
    } else if (extension == "mpkg") { 
        return "application/vnd.apple.installer+xml";
    } else if (extension == "odp") { 	
        return "application/vnd.oasis.opendocument.presentation";
    } else if (extension == "ods") { 	
        return "application/vnd.oasis.opendocument.spreadsheet";
    } else if (extension == "odt") { 	
        return "application/vnd.oasis.opendocument.text";
    } else if (extension == "oga") { 	
        return "audio/ogg";
    } else if (extension == "ogv") { 	
        return "video/ogg";
    } else if (extension == "ogx") { 	
        return "application/ogg";
    } else if (extension == "otf") { 	
        return "font/otf";
    } else if (extension == "ppt") { 	
        return "application/vnd.ms-powerpoint";
    } else if (extension == "png") { 	
        return "image/png";
    } else if (extension == "pdf") { 	
        return "application/pdf";
    } else if (extension == "pptx") { 
        return "application/vnd.openxmlformats-officedocument.presentationml.presentation";
    } else if (extension == "rar") { 	
        return "application/x-rar-compressed";
    } else if (extension == "rtf") { 	
        return "application/rtf";
    } else if (extension == "sh") {   
        return "application/x-sh";
    } else if (extension == "svg") { 	
        return "image/svg+xml";
    } else if (extension == "swf") { 	
        return "application/x-shockwave-flash";
    } else if (extension == "tar") { 	
        return "application/x-tar";
    } else if (extension == "tif") {  
        return "image/tiff";
    } else if (extension == "tiff") { 
        return "image/tiff";
    } else if (extension == "ttf") { 	
        return "font/ttf";
    } else if (extension == "txt") { 	
        return "text/plain";
    } else if (extension == "vsd") { 	
        return "application/vnd.visio";
    } else if (extension == "wav") { 	
        return "audio/wav";
    } else if (extension == "weba") { 
        return "audio/webm";
    } else if (extension == "webm") { 
        return "video/webm";
    } else if (extension == "webp") { 
        return "image/webp";
    } else if (extension == "woff") { 
        return "font/woff";
    } else if (extension == "woff2") { 
        return "font/woff2";
    } else if (extension == "xhtml") { 
        return "application/xhtml+xml";
    } else if (extension == "xls") {   
        return "application/vnd.ms-excel";
    } else if (extension == "xlsx") {  
        return "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
    } else if (extension == "xml") { 	 
        return "application/xml";
    } else if (extension == "xul") { 	 
        return "application/vnd.mozilla.xul+xml";
    } else if (extension == "zip") { 	 
        return "application/zip";
    } else if (extension == "3gp") {   
        return "video/3gpp";
    } else if (extension == "3g2") {   
        return "video/3gpp2";
    } else if (extension == "7z") { 	 
        return "application/x-7z-compressed";
    } else {
        return "text/plain";
    }
}
