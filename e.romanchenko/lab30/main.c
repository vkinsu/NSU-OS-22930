#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/socket_example"

void handle_client(int client_socket) {
    char buffer[256];
    ssize_t read_bytes;

    // Читаем данные от клиента
    while ((read_bytes = recv(client_socket, buffer, sizeof(buffer), 0)) > 0) {
        // Преобразуем символы в верхний регистр
        for (ssize_t i = 0; i < read_bytes; ++i) {
            buffer[i] = toupper(buffer[i]);
        }

        // Выводим результат в стандартный поток вывода сервера
        write(STDOUT_FILENO, buffer, read_bytes);
    }

    close(client_socket);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_un server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // Создаем сокет
    server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Задаем параметры адреса сервера
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    // Привязываем сокет к адресу
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Слушаем на сокете
    if (listen(server_socket, 5) == -1) {
        perror("listen");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Принимаем подключения от клиентов
    while ((client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len)) != -1) {
        handle_client(client_socket);
    }

    // Закрываем сокет сервера и удаляем файл сокета
    close(server_socket);
    unlink(SOCKET_PATH);

    return 0;
}
