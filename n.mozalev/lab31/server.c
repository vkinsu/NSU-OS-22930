#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/select.h>
#include <errno.h>
#include <ctype.h>

#define SOCKET_PATH "/tmp/task31"
#define MAX_CLIENTS 5

int main() {
    int server_socket, client_sockets[MAX_CLIENTS], max_sd, activity, i, sd;
    socklen_t client_length;
    struct sockaddr_un server_address, client_address;
    fd_set readfds;
    char buffer[1024];


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


    if (listen(server_socket, MAX_CLIENTS) == -1) {
        perror("Ошибка при слушании сокета");
        exit(EXIT_FAILURE);
    }

    printf("Сервер ожидает подключения...\n");


    for (i = 0; i < MAX_CLIENTS; i++) {
        client_sockets[i] = 0;
    }

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(server_socket, &readfds);
        max_sd = server_socket;

        for (i = 0; i < MAX_CLIENTS; i++) {
            sd = client_sockets[i];

            if (sd > 0) {
                FD_SET(sd, &readfds);
            }

            if (sd > max_sd) {
                max_sd = sd;
            }
        }


        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        if ((activity < 0) && (errno != EINTR)) {
            perror("Ошибка в select");
            exit(EXIT_FAILURE);
        }


        if (FD_ISSET(server_socket, &readfds)) {
            client_length = sizeof(client_address);
            sd = accept(server_socket, (struct sockaddr*)&client_address, &client_length);
            if (sd < 0) {
                perror("Ошибка при принятии подключения");
                exit(EXIT_FAILURE);
            }

            printf("Подключено клиентом\n");


            for (i = 0; i < MAX_CLIENTS; i++) {
                if (client_sockets[i] == 0) {
                    client_sockets[i] = sd;
                    break;
                }
            }
        }


        for (i = 0; i < MAX_CLIENTS; i++) {
            sd = client_sockets[i];

            if (FD_ISSET(sd, &readfds)) {
                ssize_t bytes_received = recv(sd, buffer, sizeof(buffer), 0);
                if (bytes_received == 0) {

                    printf("Клиент отключен\n");
                    close(sd);
                    client_sockets[i] = 0;
                } else {

                    buffer[bytes_received] = '\0';
                    printf("Получено от клиента %d: %s\n", i + 1, buffer);


                    for (int j = 0; j < bytes_received; j++) {
                        buffer[j] = toupper(buffer[j]);
                    }

                    printf("Преобразовано в верхний регистр: %s\n", buffer);


                    send(sd, buffer, strlen(buffer), 0);
                }
            }
        }


        sleep(2);
    }

    close(server_socket);
    for (i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i] > 0) {
            close(client_sockets[i]);
        }
    }
    unlink(SOCKET_PATH);
    printf("Сервер завершен.\n");

    return 0;
}
