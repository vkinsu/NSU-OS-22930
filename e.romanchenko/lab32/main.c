#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/epoll.h>

#define SOCKET_PATH "/tmp/socket_example"
#define MAX_EVENTS 10

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
    int server_socket, epoll_fd, client_socket;
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
    if (listen(server_socket, SOMAXCONN) == -1) {
        perror("listen");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Создаем epoll-дескриптор
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    struct epoll_event ev, events[MAX_EVENTS];
    ev.events = EPOLLIN;
    ev.data.fd = server_socket;

    // Регистрируем серверный сокет для событий EPOLLIN
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_socket, &ev) == -1) {
        perror("epoll_ctl");
        close(server_socket);
        close(epoll_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on socket: %s\n", SOCKET_PATH);

    while (1) {
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            break;
        }

        for (int i = 0; i < nfds; ++i) {
            if (events[i].data.fd == server_socket) {
                // Принимаем нового клиента
                client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
                if (client_socket == -1) {
                    perror("accept");
                    continue;
                }

                ev.events = EPOLLIN;
                ev.data.fd = client_socket;

                // Регистрируем клиентский сокет для событий EPOLLIN
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_socket, &ev) == -1) {
                    perror("epoll_ctl");
                    close(client_socket);
                    continue;
                }

                printf("New client connected\n");
            } else {
                // Обрабатываем данные от клиента
                handle_client(events[i].data.fd);

                // Убираем клиентский сокет из множества ожидающих
                epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
            }
        }
    }

    // Закрываем все сокеты и epoll-дескриптор
    close(server_socket);
    close(epoll_fd);
    unlink(SOCKET_PATH);

    return 0;
}
