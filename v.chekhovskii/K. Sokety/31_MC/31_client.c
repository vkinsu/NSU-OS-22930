#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

int main() {
    int client_fd;
    struct sockaddr_un server_addr;

    if ((client_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation error");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, "/tmp/31_server");

    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Unable to receive connection from server");
        exit(1);
    }

    char buffer[256];
    while (1) {
        printf("Connection established. \nPlease enter text you want to send to server: \n");
        fgets(buffer, sizeof(buffer), stdin);

        send(client_fd, buffer, strlen(buffer), 0);
    }

    close(client_fd);
    printf("CLIENT JOB IS DONE.\n");
    return 0;
}
