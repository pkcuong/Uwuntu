#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h> 
#define MAX 1024
#define PORT 8080
#define SA struct sockaddr

void receive_file(int sockfd, char *filename) {
    char buff[MAX];
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd < 0) {
        printf("Error creating/opening file.\n");
        return;
    }

    while (1) {
        int bytes_read = read(sockfd, buff, MAX);
        if (bytes_read <= 0) break;
        write(fd, buff, bytes_read);
    }
    close(fd);
}

int main() {
    int sockfd;
    struct sockaddr_in servaddr;

    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");

   
    receive_file(sockfd, "client_file.txt");

    
    close(sockfd);
    return 0;
}
