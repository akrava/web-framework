#include <sockets.h>
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

static bool writeDataToClient(int sckt, std::string data);
Sockets::Sockets(bool ipv6, std::string ip, int port) {
    this->ipv6 = ipv6;
    this->ip = ip;
    this->port = port;
}

Sockets::Sockets(InitParams params)
        : Sockets::Sockets(params.getIpv6(), params.getIp(), params.getPort()) {}

void Sockets::launch_demo_v01() {
    int create_socket;
    int new_socket;
    char *buffer;
    int bufsize = 1024 * 1024;
    struct sockaddr_in address;
    struct sockaddr_in6 addr_ipv6;
    socklen_t addrlen;

    buffer = (char*) malloc(bufsize);
    if (!buffer) {
        printf("The receive buffer was not allocated\n");
        exit(1);
    }

    create_socket = socket(this->ipv6 ? AF_INET6 : AF_INET, SOCK_STREAM, 0);
    if (create_socket == -1) {
        perror("The socket was not created");
        exit(1);
    }

    printf("The socket was created\n");

    if (this->ipv6) {
        // ip v6
        memset(&addr_ipv6, 0, sizeof(addr_ipv6));
        addr_ipv6.sin6_family = AF_INET6;
        inet_pton(AF_INET6, this->ip.c_str(), &(addr_ipv6.sin6_addr));
        addr_ipv6.sin6_port = htons((uint16_t)this->port);
    } else {
        memset(&address, 0, sizeof(address));
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = inet_addr(this->ip.c_str());
        address.sin_port = htons((uint16_t)this->port);
    }

    struct sockaddr * addr = this->ipv6 ? (struct sockaddr *)&addr_ipv6 : (struct sockaddr *)&address;

    if (bind(create_socket, addr, sizeof(*addr)) == -1) {
        perror("The socket was not bound");
        exit(1);
    }

    printf("The socket is bound\n");


    if (listen(create_socket, 10) == -1) {
        perror("The socket was not opened for listening");
        exit(1);
    }

    printf("The socket is listening\n");

    while (true) {

        std::string msg = "<!doctype html><html><body><center><h1>KURSA4</h1></center>";

        addrlen = sizeof(address);
        new_socket = accept(create_socket, (struct sockaddr *) &address, &addrlen);

        if (new_socket == -1) {
            perror("A client was not accepted");
            exit(1);
        }

        printf("A client is connected from %s:%hu...\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));


        int numRead = recv(new_socket, buffer, bufsize, 0);
        if (numRead < 1) {
            if (numRead == 0) {
                printf("The client was not read from: disconnected\n");
            } else {
                perror("The client was not read from");
            }
            close(new_socket);
            continue;
        }
        printf("%.*s\n", numRead, buffer);

        if (!writeDataToClient(new_socket, std::string("HTTP/1.1 200 OK\r\n"))) {
            close(new_socket);
            continue;
        }

        msg += "<h2>";
        char * beg = strstr(buffer, "GET ") + 4;
        char * end = strstr(buffer, "HTTP/1.1") - 1;
        std::string param;
        while (*beg != *end) {
            msg += *beg;
            param += *beg;
            beg++;
        }
        msg += "</h2><h3>Kak tebe takoe Elon Musk?</h3><p>Almost is ready ... </p></body></html>";

        char clen[40];
        sprintf(clen, "Content-length: %ld\r\n", msg.length());
        if (!writeDataToClient(new_socket, clen)){
            close(new_socket);
            continue;
        }

        if (!writeDataToClient(new_socket, "Content-Type: text/html; charset=utf-8\r\n")){
            close(new_socket);
            continue;
        }

        if (!writeDataToClient(new_socket, "Connection: close\r\n\r\n") == -1){
            close(new_socket);
            continue;
        }

        if (!writeDataToClient(new_socket, msg)) {
            close(new_socket);
            continue;
        }

        msg.clear();

        printf("The file was sent successfully\n");
        close(new_socket);
        if (!strcmp(param.c_str(), "/exit")) break;
    }

    close(create_socket);

}


static bool writeDataToClient(int sckt, std::string data) {
    const char *pdata = data.c_str();
    int datalen = data.length();
    while (datalen > 0) {
        int numSent = send(sckt, pdata, datalen, 0);
        if (numSent <= 0){
            if (numSent == 0){
                printf("The client was not written to: disconnected\n");
            } else {
                perror("The client was not written to");
            }
            return false;
        }
        pdata += numSent;
        datalen -= numSent;
    }

    return true;
}
