#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h> 
#define MAX 1024
#define PORT 8080
#define SA struct sockaddr

void send_file(int sockfd, char *filename) {
    char buff[MAX];
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        printf("Error opening file.\n");
        return;
    }

    while (1) {
        int bytes_read = read(fd, buff, MAX);
        if (bytes_read <= 0) break;
        write(sockfd, buff, bytes_read);
    }
    close(fd);
}

int main() {
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    int len = sizeof(cli);


    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0) {
        printf("server accept failed...\n");
        exit(0);
    }
    else
        printf("server accept the client...\n");


    send_file(connfd, "server_file.txt");

    close(sockfd);
    return 0;
}
