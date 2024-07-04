#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 4444
#define BUF_SIZE 2000
#define CLADDR_LEN 100

int main() {
    struct sockaddr_in addr, cl_addr;
    int sockfd, len, ret, newsockfd;
    char buffer[BUF_SIZE];
    pid_t childpid;
    char clientAddr[CLADDR_LEN];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }
    printf("Socket created...\n");

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT); // Convert port number to network byte order

    ret = bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
    if (ret < 0) {
        perror("Error binding");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Binding done...\n");

    printf("Waiting for a connection...\n");
    if (listen(sockfd, 5) < 0) {
        perror("Error in listen");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    for (;;) {
        len = sizeof(cl_addr);
        newsockfd = accept(sockfd, (struct sockaddr *)&cl_addr, &len);
        if (newsockfd < 0) {
            perror("Error accepting connection");
            continue;
        }
        printf("Connection accepted...\n");

        inet_ntop(AF_INET, &(cl_addr.sin_addr), clientAddr, CLADDR_LEN);
        if ((childpid = fork()) == 0) { // Creating a child process
            close(sockfd); // Stop listening for new connections by the main process

            for (;;) {
                memset(buffer, 0, BUF_SIZE);
                ret = recv(newsockfd, buffer, BUF_SIZE, 0);
                if (ret < 0) {
                    perror("Error receiving data");
                    close(newsockfd);
                    exit(EXIT_FAILURE);
                } else if (ret == 0) {
                    printf("Client disconnected\n");
                    close(newsockfd);
                    break;
                }
                printf("Received data from %s: %s\n", clientAddr, buffer);

                ret = send(newsockfd, buffer, strlen(buffer), 0);
                if (ret < 0) {
                    perror("Error sending data");
                    close(newsockfd);
                    exit(EXIT_FAILURE);
                }
                printf("Sent data to %s: %s\n", clientAddr, buffer);
            }
            exit(0);
        }
        close(newsockfd);
    }

    close(sockfd);
    return 0;
}
