#include <socket.h>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <runtime_exception.h>
#include <sys/time.h>
#define __BUFFER_SIZE 1024

using namespace std;

Socket::Socket(const char * ip, int port, bool isIPv6) {
    this->ip = ip;
    this->port = port;
    this->ipv6 = isIPv6;
    socketAddress = (sockaddr *)malloc(sizeof(sockaddr));
    memset(socketAddress, 0, sizeof(sockaddr));
}

Socket::Socket(InitParams params) : Socket::Socket(params.getIP(), params.getPort(), params.isIPv6()) {}


Socket::~Socket() {
    close(socket_fd);
    free(socketAddress);
}

void Socket::init() {
    auto * address = (sockaddr_in *)socketAddress;
    auto * address6 = (sockaddr_in6 *)socketAddress;
    socket_fd = socket(ipv6 ? AF_INET6 : AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        throw RuntimeException(string("The socket was not created: ") + strerror(errno));
    }
    int opt = 1;
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0) {
        throw RuntimeException(string("The options for socket were not set: ") + strerror(errno));
    }
    cout << "The socket was created" << endl;
    if (ipv6) {
        address6->sin6_family = AF_INET6;
        inet_pton(AF_INET6, ip.c_str(), &address6->sin6_addr);
        address6->sin6_port = htons((uint16_t)port);
    } else {
        address->sin_family = AF_INET;
        address->sin_addr.s_addr = inet_addr(ip.c_str());
        address->sin_port = htons((uint16_t)port);
    }
    if (bind(socket_fd, socketAddress, ipv6 ? sizeof(sockaddr_in6) : sizeof(sockaddr_in)) == -1) {
        throw RuntimeException(string("The socket was not bound: ") + strerror(errno));
    }
    cout << "The socket is bound" << endl;
    if (listen(socket_fd, 10) == -1) {
        throw RuntimeException(string("The socket was not opened for listening: ") + strerror(errno));
    }
    cout << "The socket is listening" << endl;
}

std::string Socket::getData() {
    sockaddr_in sender{};
    socklen_t len = sizeof(sender);
    client_fd = accept(socket_fd, (sockaddr *)&sender, &len);
    if (client_fd == -1) {
        throw RuntimeException(string("A client was not accepted: ") + strerror(errno));
    }
    cout << "A client is connected from " << inet_ntoa(sender.sin_addr) << ":"
         << to_string(ntohs(sender.sin_port)) << endl;
    string data;
    auto * buffer = new char[__BUFFER_SIZE + 1];
    ssize_t numRead = recv(client_fd, buffer, __BUFFER_SIZE, 0);
    if (numRead < 0) {
        perror("The client was not read from");
        close(client_fd);
        delete [] buffer;
        return string();
    }
    buffer[numRead] = 0;
    data += buffer;
    size_t length = 0;
    size_t cur = 0;
    size_t startKey = data.find("Content-Length: ");
    size_t endValue = data.find("\r\n", startKey + 16);
    size_t startBody = data.find("\r\n\r\n");
    if (startKey != string::npos && endValue != string::npos && startBody != string::npos) {
        try {
            string length_str = data.substr(startKey + 16, endValue - startKey - 16);
            length = stoul(length_str);
            cur = data.length() - startBody - 3;
        } catch (out_of_range & err) {
        } catch (invalid_argument & err) {
        }
    }
    while (cur < length) {
        ssize_t length_cur = recv(client_fd, buffer, 1024, 0);
        if (numRead < 1) {
            perror("The client was not read from");
            close(client_fd);
            delete[] buffer;
            return std::string();
        }
        buffer[length_cur] = 0;
        cur += length_cur;
        data += buffer;
    }
    delete [] buffer;
    return data;
}

void Socket::receiveData(string & data) {
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

std::string Socket::toString() {
    return "ip: " + ip + " port: " + to_string(port);
}
