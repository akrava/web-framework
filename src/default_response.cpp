#include "default_response.h"

using namespace std;

DefaultResponse::DefaultResponse(int status_code, const char *body) : statusCode(status_code) {
    string title = status_code < 0 ? body : to_string(status_code) + " "
                                                              + HTTP::getReasonPhrase(status_code);
    getHeaders()->add("Content-Type", "text/html; charset=utf-8");
    string data = "<html><head><title>";
    data += title;
    data += "</title></head><body><h1>";
    data += title;
    data += "</h1><hr><address>Web-framework (c++) 2019</address></body></html>";
    getBody()->setBody(data);
}
