#include <iostream>
#include "network.h"

using namespace std;

Network::Network(const char *ip, int port, bool isIPv6) {
    this->socketImpl = nullptr;
    this->ip = ip;
    this->port = port;
    this->isIpv6 = isIPv6;
}

Network::Network(InitParams params) {
    this->socketImpl = nullptr;
    this->ip = params.getIP();
    this->port = params.getPort();
    this->isIpv6 = params.isIPv6();
}

void Network::setAPI(SocketAPI * api) {
    if (api) {
        this->socketImpl = api;
    }
}

bool Network::init() {
    if (!socketImpl) {
        cerr << "Socket implementation is null" << endl;
        return false;
    }
    socketImpl->init(ip, port, isIpv6);
    return true;
}

std::string Network::readData() {
    if (!socketImpl) {
        cerr << "Socket implementation is null" << endl;
        exit(EXIT_FAILURE);
    }
    return this->socketImpl->getData();
}

bool Network::receiveData(const std::string & data) {
    if (!socketImpl) {
        cerr << "Socket implementation is null" << endl;
        return false;
    }
    this->socketImpl->receiveData(data);
    return true;
}

std::string Network::toString() {
    if (!socketImpl) {
        cerr << "Socket implementation is null" << endl;
        return nullptr;
    }
    return "ip: " + ip + " port: " + to_string(port);
}


