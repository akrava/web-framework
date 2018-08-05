#include <response.h>

using namespace std;

Response::Response() {
	version = HTTP::Version::HTTP_UNDEFINED;
	status = 501;
	headers = new Headers();
	body = new MessageBody();
}

Response::Response(const char * body) {
	version = HTTP::Version::HTTP_1_1;
	status = 200;
	headers = new Headers();
	this->body = new MessageBody(body);
}

Response::Response(HTTP::Version version, int status, Headers & headers, MessageBody & body) {
	this->version = version;
	this->status = status;
	this->headers = new Headers(headers);
	this->body = new MessageBody(body);
}

void Response::setVersion(HTTP::Version version) {
	this->version = version;
}

void Response::setStatus(int status) {
	this->status = status;
}

void Response::setHeaders(Headers & headers) {
	delete this->headers;
	this->headers = new Headers(headers);
}

void Response::setBody(MessageBody & body) {
	delete this->body;
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

Response::~Response() {
	delete headers;
	headers = nullptr;
	delete body;
	body = nullptr;
}
