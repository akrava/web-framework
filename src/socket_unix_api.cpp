#ifdef __linux__
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <runtime_exception.h>
#include <sys/time.h>
#include <netdb.h>
#include "socket_unix_api.h"
#define __BUFFER_SIZE 2048

using namespace std;

static void copyToString(std::string & data, const char * input, int len);
static size_t getContentLength(const std::string & data);
static size_t getCurrentLength(const std::string & data);
static bool endsWithTwoNewLines(const std::string & data);

SocketUnixAPI::~SocketUnixAPI() {
    close(socket_fd);
    free(socketAddress);
}

void SocketUnixAPI::init(std::string ip, int port, bool isIpv6) {
    socketAddress = (sockaddr *)malloc(sizeof(sockaddr));
    memset(socketAddress, 0, sizeof(sockaddr));
    auto * address = (sockaddr_in *)socketAddress;
    auto * address6 = (sockaddr_in6 *)socketAddress;
    socket_fd = socket(isIpv6 ? AF_INET6 : AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        throw RuntimeException(string("The socket was not created: ") + strerror(errno));
    }
    int opt = 1;
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0) {
        throw RuntimeException(string("The options for socket were not set: ") + strerror(errno));
    }
    cout << "The socket was created" << endl;
    if (isIpv6) {
        address6->sin6_family = AF_INET6;
        inet_pton(AF_INET6, ip.c_str(), &address6->sin6_addr);
        address6->sin6_port = htons((uint16_t)port);
    } else {
        address->sin_family = AF_INET;
        address->sin_addr.s_addr = inet_addr(ip.c_str());
        address->sin_port = htons((uint16_t)port);
    }
    if (bind(socket_fd, socketAddress, isIpv6 ? sizeof(sockaddr_in6) : sizeof(sockaddr_in)) == -1) {
        throw RuntimeException(string("The socket was not bound: ") + strerror(errno));
    }
    cout << "The socket is bound" << endl;
    if (listen(socket_fd, 10) == -1) {
        throw RuntimeException(string("The socket was not opened for listening: ") + strerror(errno));
    }
    cout << "The socket is listening" << endl;
}

std::string SocketUnixAPI::getData() {
    sockaddr_in sender{};
    socklen_t len = sizeof(sender);
    client_fd = accept(socket_fd, (sockaddr *)&sender, &len);
    if (client_fd == -1) {
        throw RuntimeException(string("A client was not accepted: ") + strerror(errno));
    }
    cout << "A client is connected from " << inet_ntoa(sender.sin_addr) << ":"
         << to_string(ntohs(sender.sin_port)) << endl;
    char buffer[__BUFFER_SIZE];
    string data;
    size_t length = string::npos;
    size_t cur = 0;
    do {
        size_t numRead = recv(client_fd, buffer, __BUFFER_SIZE, 0);
        if (numRead == 0) {
            break;
        } else if (numRead < 0) {
            perror("The client was not read from");
            close(client_fd);
            return string();
        }
        copyToString(data, buffer, numRead);
        if (length == string::npos) {
            length = getContentLength(data);
        }
        if (cur == 0) {
            cur = getCurrentLength(data);
        } else {
            cur += numRead;
        }
        if (cur == 0 && length == string::npos && endsWithTwoNewLines(data)) {
            break;
        }
    } while (cur < length);
    return data;
}

void SocketUnixAPI::receiveData(const std::string &data) {
    auto * p_data = data.c_str();
    size_t dataLength = data.length();
    while (dataLength > 0) {
        ssize_t numSent = send(client_fd, p_data, dataLength, 0);
        if (numSent < 0) {
            perror("The client was not written to");
            close(client_fd);
            return;
        }
        p_data += numSent;
        dataLength -= numSent;
    }
    close(client_fd);
    cout << "received " << data.length() << " bytes of data" << endl;
}

 string SocketUnixAPI::getIpFromDomain(string &domain, bool isHttps, bool *IPv6) {
	addrinfo hints, *serverInfo;
	int error;
	string ip;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	if ((error = getaddrinfo(domain.c_str(), isHttps ? "https" : "http", &hints, &serverInfo)) != 0) {
		string err = "Error while initializing: Cannot get ip: ";
		err += gai_strerror(error);
		throw RuntimeException(err);
	}
	for (auto * temp = serverInfo; temp != nullptr; temp = temp->ai_next) {
		if (temp->ai_flags == AI_PASSIVE && !temp->ai_next) continue;
		auto *h = (struct sockaddr_in *)temp->ai_addr;
		ip = inet_ntoa(h->sin_addr);
		*IPv6 = h->sin_family == AF_INET6;
		if (temp->ai_next && temp->ai_next->ai_flags == AI_PASSIVE && !temp->ai_next->ai_next) break;
	}
	freeaddrinfo(serverInfo);
	return ip;
}

void copyToString(std::string & data, const char * input, int len) {
    string temp;
    temp.assign(input, len);
    data += temp;
}

size_t getContentLength(const std::string & data) {
    size_t startKey = data.find("Content-Length: ");
    size_t endValue = data.find("\r\n", startKey + 16);
    if (startKey != string::npos && endValue != string::npos) {
        try {
            string length_str = data.substr(startKey + 16, endValue - startKey - 16);
            return stoul(length_str);
        }
        catch (out_of_range & err) {
        }
        catch (invalid_argument & err) {
        }
    }
    return string::npos;
}

size_t getCurrentLength(const std::string & data) {
    size_t startBody = data.find("\r\n\r\n");
    if (startBody != string::npos) {
        return data.length() - startBody - 4;
    }
    return 0;
}

bool endsWithTwoNewLines(const std::string & data) {
    size_t position = data.find("\r\n\r\n");
    if (position == string::npos) {
        return false;
    }
    return position + 4 == data.length();
}


#endif // __linux__
