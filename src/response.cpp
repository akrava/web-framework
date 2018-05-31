#include <response.h>
#include <message_body.h>
#include <headers.h>
#include <http.h>

Response::Response() :  body() {
	version = HTTP::Version::HTTP_UNDEFINED;
	status = 501;
	headers = new Headers();
	body = new MessageBody();
}

Response::Response(HTTP::Version version, int status, Headers & headers,
				   MessageBody & body) : body(new MessageBody(body)) {
	this->version = version;
	this->status = status;
	this->headers = new Headers(headers);
}

void Response::setVersion(HTTP::Version version) {
	this->version = version;
}

void Response::setStatus(int status) {
	this->status = status;
}

void Response::setHeaders(Headers & headers) {
	this->headers = new Headers(headers);
}

void Response::setBody(MessageBody & body) {
	this->body = new MessageBody(body);
}

HTTP::Version Response::getVersion() {
	return version;
}

int Response::getStatus() {
	return status;
}

Headers * Response::getHeaders() {
	return headers;
}

MessageBody * Response::getBody() {
	return body;
}
