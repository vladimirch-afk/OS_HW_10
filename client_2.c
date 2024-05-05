#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_MESSAGE_SIZE 1024

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr,"Usage: %s <server_ip> <server_port>\n", argv[0]);
        exit(1);
    }

    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    char buffer[MAX_MESSAGE_SIZE];

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("ERROR opening socket");
    }

    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);

    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
        error("Invalid address");
    }

    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        error("ERROR connecting");
    }

    printf("Connected to server. Waiting for messages from client 1.\n");
    while (1) {
        bzero(buffer, MAX_MESSAGE_SIZE);
        n = read(sockfd, buffer, MAX_MESSAGE_SIZE);
        if (n < 0) {
            error("ERROR reading from socket");
        }
        printf("Client 1: %s", buffer);
        if (strcmp(buffer, "The End\n") == 0) {
            break;
        }
    }

    close(sockfd);
    return 0;
}

