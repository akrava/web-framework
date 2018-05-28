#include "uri.h"

using namespace std;

URI::URI() {
    raw_data = string();
    uri = string();
    params =  unordered_map<string, string>();
}

URI::URI(std::string & uri) {
    raw_data = uri;
    this->uri = uri;
    params =  unordered_map<string, string>();
}

void URI::setRawData(string & uri) {
    raw_data = uri;
    params =  unordered_map<string, string>();
    this->uri = uri;
}

string URI::getUri() {
    return uri;
}

unordered_map<std::string, std::string> URI::getParams() {
    return params;
}
