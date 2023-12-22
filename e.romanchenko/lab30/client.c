#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/socket_example"

int main() {
    int client_socket;
    struct sockaddr_un server_addr;

    // Создаем сокет клиента
    client_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Задаем параметры адреса сервера
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    // Устанавливаем соединение с сервером
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // Отправляем текст на сервер
    const char *text = "Hello, Server!";
    if (send(client_socket, text, strlen(text), 0) == -1) {
        perror("send");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // Закрываем сокет клиента
    close(client_socket);

    return 0;
}
