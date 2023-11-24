#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <ctype.h>

#define SOCKET_PATH "/tmp/my_unix_socket"
#define BUFFER_SIZE 256

int main() {
    // Создание Unix domain сокета
    int server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Установка параметров структуры sockaddr_un
    struct sockaddr_un server_addr;
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    // Привязка сокета к адресу
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error binding socket");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Прослушивание сокета
    if (listen(server_socket, 5) == -1) {
        perror("Error listening on socket");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on socket: %s\n", SOCKET_PATH);

    // Ожидание подключения клиента
    int client_socket = accept(server_socket, NULL, NULL);
    if (client_socket == -1) {
        perror("Error accepting connection");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Client connected\n");

    // Чтение и преобразование текста от клиента
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    while ((bytes_read = read(client_socket, buffer, BUFFER_SIZE)) > 0) {
        for (ssize_t i = 0; i < bytes_read; i++) {
            buffer[i] = toupper(buffer[i]);
        }

        write(STDOUT_FILENO, buffer, bytes_read);
    }

    // Закрытие сокетов
    close(client_socket);
    close(server_socket);
    unlink(SOCKET_PATH);

    printf("Server shutting down\n");

    return 0;
}
