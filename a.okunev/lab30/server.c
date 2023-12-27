#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <ctype.h>

#define SOCKET_PATH "/tmp/socket_example"

int main() {
    int server_socket, client_socket;
    socklen_t client_length;
    struct sockaddr_un server_address, client_address;


    server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Ошибка создания сокета");
        exit(EXIT_FAILURE);
    }


    memset(&server_address, 0, sizeof(server_address));
    server_address.sun_family = AF_UNIX;
    strcpy(server_address.sun_path, SOCKET_PATH);


    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("Ошибка привязки сокета");
        exit(EXIT_FAILURE);
    }


    if (listen(server_socket, 5) == -1) {
        perror("Ошибка при слушании сокета");
        exit(EXIT_FAILURE);
    }

    printf("Сервер ожидает подключения...\n");


    client_length = sizeof(client_address);
    client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_length);
    if (client_socket == -1) {
        perror("Ошибка при принятии подключения");
        exit(EXIT_FAILURE);
    }

    printf("Подключено клиентом\n");


    char buffer[1024];
    ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    buffer[bytes_received] = '\0';

    printf("Получено от клиента: %s\n", buffer);


    for (int i = 0; i < strlen(buffer); i++) {
        buffer[i] = toupper(buffer[i]);
    }

    printf("Преобразовано в верхний регистр: %s\n", buffer);


    send(client_socket, buffer, strlen(buffer), 0);


    close(client_socket);
    close(server_socket);


    unlink(SOCKET_PATH);

    printf("Сервер завершен.\n");

    return 0;
}