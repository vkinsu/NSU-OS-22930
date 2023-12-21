#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/socket_example"

int setup_client_socket() {
    int client_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_un server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    return client_socket;
}

void send_text(int client_socket, const char *text) {
    if (send(client_socket, text, strlen(text), 0) == -1) {
        perror("send");
        close(client_socket);
        exit(EXIT_FAILURE);
    }
}

int main() {
    int client_socket = setup_client_socket();
    printf("Client connected\n");

    const char *text = "Hello, Server!";
    send_text(client_socket, text);

    close(client_socket);

    return 0;
}
