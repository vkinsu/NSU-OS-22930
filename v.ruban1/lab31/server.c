#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <ctype.h>
#include <sys/time.h>
#include <sys/types.h>

#define SOCKET_PATH "/tmp/my_unix_socket"
#define BUFFER_SIZE 256
#define MAX_CLIENTS 5

int main() {
    unlink(SOCKET_PATH);
    int server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_un server_addr;
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error binding socket");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, MAX_CLIENTS) == -1) {
        perror("Error listening on socket");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on socket: %s\n", SOCKET_PATH);

    fd_set active_fd_set, read_fd_set;
    FD_ZERO(&active_fd_set);
    FD_SET(server_socket, &active_fd_set);

    while (1) {
        read_fd_set = active_fd_set;

        if (select(FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) == -1) {
            perror("Error in select");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < FD_SETSIZE; ++i) {
            if (FD_ISSET(i, &read_fd_set)) {
                if (i == server_socket) {
                    int client_socket = accept(server_socket, NULL, NULL);
                    if (client_socket == -1) {
                        perror("Error accepting connection");
                        close(server_socket);
                        exit(EXIT_FAILURE);
                    }

                    printf("New client connected\n");

                    FD_SET(client_socket, &active_fd_set);
                } else {
                    char buffer[BUFFER_SIZE];
                    ssize_t bytes_read = read(i, buffer, BUFFER_SIZE);

                    if (bytes_read > 0) {
                        for (ssize_t j = 0; j < bytes_read; j++) {
                            buffer[j] = toupper(buffer[j]);
                        }

                        write(STDOUT_FILENO, buffer, bytes_read);
                    } else {
                        printf("Client disconnected\n");
                        close(i);
                        FD_CLR(i, &active_fd_set);
                    }
                }
            }
        }
    }

    return 0;
}
