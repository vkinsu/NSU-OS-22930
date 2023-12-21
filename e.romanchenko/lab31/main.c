#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/time.h>

#define SOCKET_PATH "/tmp/socket_example"
#define MAX_CLIENTS 10

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
    int server_socket, client_socket[MAX_CLIENTS];
    fd_set read_fds, master_fds;
    struct sockaddr_un server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int max_fd, activity, i;

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
    if (listen(server_socket, MAX_CLIENTS) == -1) {
        perror("listen");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Инициализируем множество дескрипторов для мультиплексирования
    FD_ZERO(&master_fds);
    FD_SET(server_socket, &master_fds);
    max_fd = server_socket;

    printf("Server is listening on socket: %s\n", SOCKET_PATH);

    while (1) {
        read_fds = master_fds;
        activity = select(max_fd + 1, &read_fds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR)) {
            perror("select");
            break;
        }

        // Проверяем новые подключения
        if (FD_ISSET(server_socket, &read_fds)) {
            client_socket[i] = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
            if (client_socket[i] == -1) {
                perror("accept");
                continue;
            }

            FD_SET(client_socket[i], &master_fds);

            if (client_socket[i] > max_fd) {
                max_fd = client_socket[i];
            }

            printf("New client connected\n");
        }

        // Обрабатываем данные от клиентов
        for (i = 0; i < MAX_CLIENTS; ++i) {
            if (FD_ISSET(client_socket[i], &read_fds)) {
                handle_client(client_socket[i]);
                FD_CLR(client_socket[i], &master_fds);
            }
        }
    }

    // Закрываем все сокеты
    for (i = 0; i < MAX_CLIENTS; ++i) {
        if (client_socket[i] != -1) {
            close(client_socket[i]);
        }
    }

    close(server_socket);
    unlink(SOCKET_PATH);

    return 0;
}

