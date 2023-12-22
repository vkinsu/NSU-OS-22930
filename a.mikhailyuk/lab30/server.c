#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <sys/un.h> 

char myToupper(char a) {
    if (a >= 97) {
        return a - 32;
    }
    else return a;
}

int main() { 
    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        perror("socket"); 
        exit(EXIT_FAILURE); 
    } 

    unlink("/tmp/socket");

    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, "/tmp/socket", sizeof(addr.sun_path) - 1);

    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
 
    if (listen(sockfd, 5) == -1) {
        perror("listen");
        close(sockfd);
        exit(EXIT_FAILURE);
    } 

    printf("Сервер ожидает подключения...\n"); 
 
    // Принятие подключения 
    int client_sockfd = accept(sockfd, NULL, NULL);

    if (client_sockfd == -1) {
        perror("accept");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
 
    printf("Клиент подключен!\n"); 

    char buf;

    while (read(client_sockfd, &buf, 1) != 0) {
        printf("%c", myToupper(buf));
    } 

    close(client_sockfd);
    close(sockfd);

    unlink("/tmp/socket");
    return 0;
}