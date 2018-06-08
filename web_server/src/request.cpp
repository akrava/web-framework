#include <request.h>
#include <headers.h>
#include <uri.h>
#include "uri.h"

Request::Request() {
	method = HTTP::Method::UNDEFINED;
	version = HTTP::Version::HTTP_UNDEFINED;
    this->uri = new URI();
    this->headers = new Headers();
    this->body = new MessageBody();

}

Request::Request(HTTP::Method method, std::string & uri, HTTP::Version version,
				 std::string & headers, std::string & body) : headers(new Headers(headers)), body(new MessageBody(body)) {
	this->method = method;
	this->version = version;
	this->uri = new URI(uri);
}

HTTP::Method Request::getMethod() {
	return method;
}

URI * Request::getURI() {
	return uri;
}

HTTP::Version Request::getVersion() {
	return version;
}

Headers * Request::getHeaders() {
	return headers;
}

MessageBody * Request::getMessageBody() {
	return body;
}

Request::~Request() {
	delete uri;
	delete headers;
	delete body;
	uri = nullptr;
	headers = nullptr;
	body = nullptr;
}
