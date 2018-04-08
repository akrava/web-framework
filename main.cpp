#include <iostream>
#include <netinet/in.h>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>

bool writeDataToClient(int sckt, const void *data, int datalen) {
    const char *pdata = (const char*) data;

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

bool writeStrToClient(int sckt, const char *str) {
    return writeDataToClient(sckt, str, strlen(str));
}


int main (int argc, char ** argv) {

        int create_socket;
        int new_socket;
        char *buffer;
        int bufsize = 1024;
        struct sockaddr_in address;
        socklen_t addrlen;

        buffer = (char*) malloc(bufsize);
        if (!buffer) {
            printf("The receive buffer was not allocated\n");
            exit(1);
        }

        create_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (create_socket == -1) {
            perror("The socket was not created");
            exit(1);
        }

        printf("The socket was created\n");

        memset(&address, 0, sizeof(address));
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        address.sin_port = htons(80);

        if (bind(create_socket, (struct sockaddr *) &address, sizeof(address)) == -1){
            perror("The socket was not bound");
            exit(1);
        }

        printf("The socket is bound\n");

        char *msg = "<!doctype html><html><body><center><h1>KURSA4</h1></center><h3>Kak tebe takoe Elon Mask?</h3><p>Almost is ready ... </p></body></html>";


        printf("The file size is %ld\n", strlen(msg));

        if (listen(create_socket, 10) == -1){
            perror("The socket was not opened for listening");
            exit(1);
        }

        printf("The socket is listening\n");

        while (true) {

            addrlen = sizeof(address);
            new_socket = accept(create_socket, (struct sockaddr *) &address, &addrlen);

            if (new_socket == -1) {
                perror("A client was not accepted");
                exit(1);
            }

            printf("A client is connected from %s:%hu...\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

            // I will leave it as an exercise for you to implement
            // a proper HTTP request parser here...
            int numRead = recv(new_socket, buffer, bufsize, 0);
            if (numRead < 1){
                if (numRead == 0){
                    printf("The client was not read from: disconnected\n");
                } else {
                    perror("The client was not read from");
                }
                close(new_socket);
                continue;
            }
            printf("%.*s\n", numRead, buffer);

            if (!writeStrToClient(new_socket, "HTTP/1.1 200 OK\r\n")){
                close(new_socket);
                continue;
            }

            char clen[40];
            sprintf(clen, "Content-length: %ld\r\n", strlen(msg));
            if (!writeStrToClient(new_socket, clen)){
                close(new_socket);
                continue;
            }

            if (!writeStrToClient(new_socket, "Content-Type: text/html; charset=utf-8\r\n")){
                close(new_socket);
                continue;
            }

            if (!writeStrToClient(new_socket, "Connection: close\r\n\r\n") == -1){
                close(new_socket);
                continue;
            }

            if (!writeStrToClient(new_socket, msg)) {
                close(new_socket);
                continue;
            }

            printf("The file was sent successfully\n");
            close(new_socket);
        }

        close(create_socket);


        return 0;
}
