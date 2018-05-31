#include <request.h>
#include <headers.h>
#include <uri.h>
#include "uri.h"

Request::Request() : uri(), headers(), body() {
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

void Request::setMethod(HTTP::Method method) {
	this->method = method;
}

void Request::setURI(URI & uri) {
	this->uri = new URI(uri);
}

void Request::setVersion(HTTP::Version version) {
	this->version = version;
}

void Request::setHeaders(Headers & headers) {
	this->headers = new Headers(headers);
}

void Request::setMessageBody(MessageBody & body) {
	this->body = new MessageBody(body);
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
