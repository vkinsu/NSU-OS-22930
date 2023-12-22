#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <ctype.h>

void handleClient(int client_socket) {
    char buffer[256];
    ssize_t n;

    n = read(client_socket, buffer, sizeof(buffer));
    if (n < 0) {
        perror("Read error:");
        exit(1);
    }
    for (int i = 0; i < n; i++) {
        buffer[i] = toupper(buffer[i]);
    }

    printf("Conversion finished successfully! \nClient text in uppercase: \n%s\n", buffer);
    close(client_socket);
}

int main() {
    int server_fd, client_socket;
    socklen_t client_len;
    struct sockaddr_un server_addr, client_addr;

    if ((server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation error");
        exit(1);
    }

    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, "/tmp/30_server");

    unlink("/tmp/30_server"); // на всякий случай
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Socket binding error");
        exit(1);
    }

    if (listen(server_fd, 5) == -1) {
        perror("Unable to fetch information from client");
        exit(1);
    }

    client_len = sizeof(client_addr);

    if ((client_socket = accept(server_fd, (struct sockaddr *)&client_addr, &client_len)) == -1) {
        perror("Unable to receive connection from client");
        exit(1);
    }
    handleClient(client_socket);

    close(server_fd);
    unlink("/tmp/30_server");
    printf("SERVER JOB IS DONE.\n");
    return 0;
}
