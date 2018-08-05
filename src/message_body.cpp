#include <message_body.h>

using namespace std;

MessageBody::MessageBody() {
    body = string();
}

MessageBody::MessageBody(std::string & body) {
    this->body = body;
}

MessageBody::MessageBody(const char * body) {
    this->body = body;
}

void MessageBody::setBody(std::string & body) {
    this->body = body;
}

std::string MessageBody::getBody() {
    return body;
}

void MessageBody::clear() {
    body.clear();
}
