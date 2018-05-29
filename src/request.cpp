#include <request.h>
#include <headers.h>
#include <uri.h>

Request::Request() : uri(), headers(), body() {
	method = HTTP::Method::UNDEFINED;
	version = HTTP::Version::HTTP_UNDEFINED;
}

Request::Request(HTTP::Method method, std::string & URI, HTTP::Version version,
				 std::string & headers, std::string & body) : uri(URI), headers(headers), body(body) {
	this->method = method;
	this->version = version;
}

void Request::setMethod(HTTP::Method method) {
	this->method = method;
}

void Request::setURI(URI & uri) {
	this->uri = uri;
}

void Request::setVersion(HTTP::Version version) {
	this->version = version;
}

void Request::setHeaders(Headers & headers) {
	this->headers = headers;
}

void Request::setMessageBody(MessageBody & body) {
	this->body = body;
}

HTTP::Method Request::getMethod() {
	return method;
}

URI Request::getURI() {
	return uri;
}

HTTP::Version Request::getVersion() {
	return version;
}

Headers Request::getHeaders() {
	return headers;
}

MessageBody Request::getMessageBody() {
	return body;
}
