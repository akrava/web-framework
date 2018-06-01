#include <socket.h>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <resolv.h>
#include <string>
#include <cstring>
#include <termio.h>
#include <fcntl.h>
#include <string>
#include <cerrno>
#include <runtime_exception.h>
#include <sys/time.h>

using namespace std;

static bool writeDataToClient(int sckt, std::string data);

Socket::Socket(std::string ip, int port, bool isIPv6) {
    this->ip = ip;
    this->port = port;
    this->ipv6 = isIPv6;
    socketAddress = (sockaddr *)malloc(sizeof(sockaddr));
    memset(socketAddress, 0, sizeof(sockaddr));
}

Socket::Socket(InitParams params)
        : Socket::Socket(params.getIP(), params.getPort(), params.isIPv6()) {}


Socket::~Socket() {
    close(socket_fd);
}

void Socket::init() {
    auto * address = (sockaddr_in *)socketAddress;
    auto * address6 = (sockaddr_in6 *)socketAddress;

    socket_fd = socket(ipv6 ? AF_INET6 : AF_INET, SOCK_STREAM, 0);

    if (socket_fd == -1) {
        //ToDo log
        throw RuntimeException(string("The socket was not created: ") + strerror(errno));
    }

    int opt = 1;
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0) {
        //ToDo log
        throw RuntimeException(string("The options for socket were not set: ") + strerror(errno));
    }

    // todo log
    printf("The socket was created\n");
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
        //ToDo log
        throw RuntimeException(string("The socket was not bound: ") + strerror(errno));
    }

    // todo log
    printf("The socket is bound\n");

    if (listen(socket_fd, 10) == -1) {
        //ToDo log
        throw RuntimeException(string("The socket was not opened for listening: ") + strerror(errno));
    }

    // todo log
    printf("The socket is listening\n");
}

std::string Socket::getData() {
    sockaddr_in sender{};
    socklen_t len = sizeof(sender);

    client_fd = accept(socket_fd, (sockaddr *)&sender, &len);

    if (client_fd == -1) {
        //ToDo log
        throw RuntimeException(string("A client was not accepted: ") + strerror(errno));
    }

    //ToDo log
    printf("A client is connected from %s:%hu...\n", inet_ntoa(sender.sin_addr), ntohs(sender.sin_port));

    string data;

    auto * buffer = new char[1024];

    int numRead = recv(client_fd, buffer, 1024, 0);
    if (numRead < 1) {
        if (numRead == 0) {
            printf("The client was not read from: disconnected\n"
                   "The client was not read from: disconnected\n"
                   "The client was not read from: disconnected\n"
                   "The client was not read from: disconnected\nThe client was not read from: disconnected\nThe client was not read from: disconnected\nThe client was not read from: disconnected\nThe client was not read from: disconnected\nThe client was not read from: disconnected\n");
        } else {
            perror("The client was not read from");
        }
        perror("The client was not read from");
        close(client_fd);
        return std::string();
    }
    buffer[numRead] = 0;
    data += buffer;
    //printf("%.*s\n", numRead, buffer);

   // std::cout << data;
    std::cout << std::endl << "OK" << std::endl;
    return data;
}

void Socket::reciveData(std::string & data) {
    auto * p_data = data.c_str();
    //std::cout << std::endl << p_data << std::endl;
    size_t dataLength = data.length();
    while (dataLength > 0) {
        ssize_t numSent = send(client_fd, p_data, dataLength, 0);
        if (numSent <= 0) {
            if (numSent == 0){
                printf("The client was not written to: disconnected\n");
            } else {
                perror("The client was not written to");
            }
            close(client_fd);
            return;
        }
        p_data += numSent;
        dataLength -= numSent;
    }
    close(client_fd);
    std::cout << "recived" << std::endl;
}


//
//void Socket::launch_demo_v01() {
//    int create_socket;
//    int new_socket;
//    char *buffer;
//    int bufsize = 1024 * 1024;
//    struct sockaddr_in address;
//    struct sockaddr_in6 addr_ipv6;
//    socklen_t addrlen;
//
//    buffer = (char*) malloc(bufsize);
//    if (!buffer) {
//        printf("The receive buffer was not allocated\n");
//        exit(1);
//    }
//
//    create_socket = socket(this->ipv6 ? AF_INET6 : AF_INET, SOCK_STREAM, 0);
//    if (create_socket == -1) {
//        perror("The socket was not created");
//        exit(1);
//    }
//
//    printf("The socket was created\n");
//
//    if (this->ipv6) {
//        // host v6
//        memset(&addr_ipv6, 0, sizeof(addr_ipv6));
//        addr_ipv6.sin6_family = AF_INET6;
//        inet_pton(AF_INET6, this->ip.c_str(), &(addr_ipv6.sin6_addr));
//        addr_ipv6.sin6_port = htons((uint16_t)this->port);
//    } else {
//        memset(&address, 0, sizeof(address));
//        address.sin_family = AF_INET;
//        address.sin_addr.s_addr = inet_addr(this->ip.c_str());
//        address.sin_port = htons((uint16_t)this->port);
//    }
//
//    struct sockaddr * addr = this->ipv6 ? (struct sockaddr *)&addr_ipv6 : (struct sockaddr *)&address;
//
//    if (bind(create_socket, addr, sizeof(*addr)) == -1) {
//        perror("The socket was not bound");
//        exit(1);
//    }
//
//    printf("The socket is bound\n");
//
//
//    if (listen(create_socket, 10) == -1) {
//        perror("The socket was not opened for listening");
//        exit(1);
//    }
//
//    printf("The socket is listening\n");
//
//    while (true) {
//
//        std::string msg = "<!doctype html><html><body><center><h1>KURSA4</h1></center>";
//
//        addrlen = sizeof(address);
//        new_socket = accept(create_socket, (struct sockaddr *) &address, &addrlen);
//
//        if (new_socket == -1) {
//            perror("A client was not accepted");
//            exit(1);
//        }
//
//        printf("A client is connected from %s:%hu...\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
//
//
//        int numRead = recv(new_socket, buffer, bufsize, 0);
//        if (numRead < 1) {
//            if (numRead == 0) {
//                printf("The client was not read from: disconnected\n");
//            } else {
//                perror("The client was not read from");
//            }
//            close(new_socket);
//            continue;
//        }
//        printf("%.*s\n", numRead, buffer);
//
//        if (!writeDataToClient(new_socket, std::string("HTTP/1.1 200 OK\r\n"))) {
//            close(new_socket);
//            continue;
//        }
//
//        msg += "<h2>";
//        char * beg = strstr(buffer, "GET ") + 4;
//        char * end = strstr(buffer, "HTTP/1.1") - 1;
//        std::string param;
//        while (*beg != *end) {
//            msg += *beg;
//            param += *beg;
//            beg++;
//        }
//        msg += "</h2><h3>Kak tebe takoe Elon Musk?</h3><p>Almost is ready ... </p></body></html>";
//
//        char clen[40];
//        sprintf(clen, "Content-length: %ld\r\n", msg.length());
//        if (!writeDataToClient(new_socket, clen)){
//            close(new_socket);
//            continue;
//        }
//
//        if (!writeDataToClient(new_socket, "Content-Type: text/html; charset=utf-8\r\n")){
//            close(new_socket);
//            continue;
//        }
//
//        if (!writeDataToClient(new_socket, "Connection: close\r\n\r\n") == -1){
//            close(new_socket);
//            continue;
//        }
//
//        if (!writeDataToClient(new_socket, msg)) {
//            close(new_socket);
//            continue;
//        }
//
//        msg.clear();
//
//        printf("The file was sent successfully\n");
//        close(new_socket);
//        if (!strcmp(param.c_str(), "/exit")) break;
//    }
//
//    close(create_socket);
//
//}
//
//
//static bool writeDataToClient(int sckt, std::string data) {
//    const char *pdata = data.c_str();
//    int datalen = data.length();
//    while (datalen > 0) {
//        int numSent = send(sckt, pdata, datalen, 0);
//        if (numSent <= 0){
//            if (numSent == 0){
//                printf("The client was not written to: disconnected\n");
//            } else {
//                perror("The client was not written to");
//            }
//            return false;
//        }
//        pdata += numSent;
//        datalen -= numSent;
//    }
//
//    return true;
//}
