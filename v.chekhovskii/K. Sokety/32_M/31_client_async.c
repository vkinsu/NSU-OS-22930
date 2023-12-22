#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/epoll.h>
#include <ctype.h>

int set_nonblocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    return 0;
}

int main() {
    int client_fd; // десктриптор сокета (нужен для выполнения операций с сокетом)
    struct sockaddr_un server_addr; // адрес сервера

    // создание структуры сокета (серверного)
    if ((client_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation error");
        exit(1);
    }

    // создание сокета (клиентского)
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, "/tmp/32_server");

    // установка соединения с сервером
    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Unable to connect to server");
        exit(1);
    }

    // установка сокета в неблокирующий режим
    // (нужно для асинхронной обработки событий)
    if (set_nonblocking(client_fd) == -1) {
        perror("Unable to set socket in non-blocking mode");
        exit(1);
    }

    printf("CONNECTION ESTABLISHED.\n\n");

    char buffer[256];
    ssize_t bytes_sent;
    while (1) { // обработка сообщений (отправка)
        //send(client_fd, "test", strlen("test"), 0);
        printf("Please enter text you want to send to server: \n");
        fgets(buffer, sizeof(buffer), stdin);

        bytes_sent = send(client_fd, buffer, strlen(buffer), 0);

        if (bytes_sent == -1 && errno != EAGAIN && errno != EWOULDBLOCK) {
            printf("Unable to send text: Server is offline or not responding\n");
        }
        printf("\n");
    }

    // закрытие сокета (клиентского)
    close(client_fd);
    printf("\nCLIENT JOB IS DONE.\n");
    return 0;
}
