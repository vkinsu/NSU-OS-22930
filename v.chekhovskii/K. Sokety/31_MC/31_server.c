#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <ctype.h>

int main() {
    int server_fd, client_fd, max_fd, activity, valread;
    fd_set read_fds;

    struct sockaddr_un server_addr, client_addr;

    if ((server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation error");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, "/tmp/31_server");

    unlink("/tmp/31_server"); // на всякий случай
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Socket binding error");
        exit(1);
    }

    if (listen(server_fd, FD_SETSIZE) == -1) {
        perror("Unable to fetch information from client");
        exit(1);
    }

    FD_ZERO(&read_fds);
    FD_SET(server_fd, &read_fds);
    max_fd = server_fd;

    char buffer[256] = {0};
    while (1) {
        fd_set tmp_fds = read_fds;

        if ((activity = select(max_fd + 1, &tmp_fds, NULL, NULL, NULL)) < 0) {
            perror("Unable to fetch activity");
            exit(1);
        }

        if (FD_ISSET(server_fd, &tmp_fds)) {
            socklen_t client_len = sizeof(client_addr);
            if ((client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len)) == -1) {
                perror("Unable to accept connection from client");
                exit(1);
            }

            FD_SET(client_fd, &read_fds);
            if (client_fd > max_fd) {
                max_fd = client_fd;
            }

            printf("New connection established.\n");
        }

        for (int i = 0; i <= max_fd; i++) {
            if (FD_ISSET(i, &tmp_fds)) {
                if ((valread = read(i, buffer, sizeof(buffer))) == 0) {
                    close(i);
                    FD_CLR(i, &read_fds);
                    printf("Connection closed by client.\n");
                } else {
                    int j = 0;
                    for (j; j < valread; j++) {
                        buffer[j] = toupper(buffer[j]);
                    }
                    buffer[j] = '\0';
                    printf("New message from client!\n%s\n", buffer);
                }
            }
        }
    }

    close(server_fd);
    unlink("/tmp/31_server");
    printf("SERVER JOB IS DONE.\n");
    return 0;
}
