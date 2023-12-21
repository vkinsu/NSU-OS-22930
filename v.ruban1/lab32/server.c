#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ev.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <ctype.h>

#define SOCKET_PATH "/tmp/my_unix_socket"
#define BUFFER_SIZE 256
#define MAX_CLIENTS 5

struct client_info {
    ev_io watcher;
    int client_socket;
};

static void read_cb(EV_P_ ev_io *w, int revents) {
struct client_info *client = (struct client_info *)w;
char buffer[BUFFER_SIZE];
ssize_t bytes_read = read(client->client_socket, buffer, BUFFER_SIZE);

if (bytes_read > 0) {
for (ssize_t j = 0; j < bytes_read; j++) {
buffer[j] = toupper(buffer[j]);
}

write(STDOUT_FILENO, buffer, bytes_read);
} else {
printf("Client disconnected\n");
close(client->client_socket);
ev_io_stop(EV_A_ w);
free(client);
}
}

static void accept_cb(EV_P_ ev_io *w, int revents) {
int server_socket = w->fd;

struct sockaddr_un client_addr;
socklen_t client_len = sizeof(client_addr);
int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);

if (client_socket == -1) {
perror("Error accepting connection");
return;
}

printf("New client connected\n");

struct client_info *client = (struct client_info *)malloc(sizeof(struct client_info));
if (!client) {
perror("Error allocating memory for client");
close(client_socket);
return;
}

ev_io_init(&client->watcher, read_cb, client_socket, EV_READ);
ev_io_start(EV_A_ &client->watcher);
client->client_socket = client_socket;
}

int main() {
    int server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_un server_addr;
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
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

    struct ev_loop *loop = EV_DEFAULT;
    ev_io server_watcher;
    ev_io_init(&server_watcher, accept_cb, server_socket, EV_READ);
    ev_io_start(loop, &server_watcher);

    ev_run(loop, 0);

    close(server_socket);
    unlink(SOCKET_PATH);

    printf("Server shutting down\n");

    return 0;
}
