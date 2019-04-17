#include <request.h>

using namespace std;

Request::Request() {
	method = HTTP::Method::UNDEFINED;
	version = HTTP::Version::HTTP_UNDEFINED;
    this->uri = new URI();
    this->headers = new Headers();
    this->body = new MessageBody();
}

Request::Request(HTTP::Method method, std::string & uri, HTTP::Version version,
				 std::string & headers, std::string & body, string raw)
{
	this->method = method;
	this->version = version;
	this->uri = new URI(uri);
	this->headers = new Headers(headers);
	this->body = new MessageBody(body);
	rawData = raw;
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

string Request::getRawData() {
    return rawData;
}
