#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

int main() {
    int client_fd;
    struct sockaddr_un server_addr;

    if ((client_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation error");
        exit(1);
    }

    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, "/tmp/30_server");

    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Unable to receive connection from server");
        exit(1);
    }

    char buffer[256];
    printf("Connection established. \nPlease enter text yuo want to convert: \n");
    fgets(buffer, sizeof(buffer), stdin);

    if (write(client_fd, buffer, strlen(buffer)) == -1) {
        perror("Write error");
        exit(1);
    }

    close(client_fd);
    printf("CLIENT JOB IS DONE.\n");
    return 0;
}
