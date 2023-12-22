#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/poll.h>

int print_received_data(char* buffer, int current_client, int rc) {
    printf("size of message: %d\n", rc);
    printf("Client with descriptor %d has sent a message: ", current_client);
    for (int i = 0; i < rc; i++)
        printf("%c", toupper(buffer[i]));
    if (*buffer == '\007') {
        printf("Client %d has disconnected\n", current_client);
        return 0;
    }
    return 1;
}

char *socket_path = "\0hidden";

int main(int argc, char *argv[]) {
    if (argc > 1) socket_path=argv[1];
    int socket_descriptor = socket(AF_UNIX, SOCK_STREAM, 0);
    printf("server descriptor id is: %d\n", socket_descriptor);
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));

    addr.sun_family = AF_UNIX;

    if (*socket_path == '\0') {
        *addr.sun_path = '\0';
        strncpy(addr.sun_path+1, socket_path+1, sizeof(addr.sun_path)-2);
    } else {
        strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path)-1);
        unlink(socket_path);
    }
    if (bind(socket_descriptor, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
        printf("Error with binding server!\nShutting down...\n");
        exit(1);
    }
    printf("socket path is: %s\n", addr.sun_path);
    if (listen(socket_descriptor, 0) != 0) {
        printf("Error with listening server!\nShutting down...\n");
        exit(1);
    }
    int poll_length = 10;
    struct pollfd poll_fds[10];
    for (int i = 0; i < poll_length; i++) {
        poll_fds[i].fd = -1;
        poll_fds[i].events = POLLIN | POLLPRI;
    }
    poll_fds->fd = socket_descriptor;
    poll_fds->events = POLLIN | POLLPRI;

    char buffer[128];
    int rc;

    while (1) {
        if ((poll(poll_fds, poll_length, -1)) == -1) {
            perror("poll error");
            exit(1);
        }
        // отслеживаем ивенты отключения.
        for (int i = 0; i < poll_length; i++) {
            //проверяем возвращенные события. в случае ошибки/отключения - закрываем файловый дискриптор.
            if ((poll_fds[i].revents & POLLERR) || (poll_fds[i].revents & POLLHUP) || (poll_fds[i].revents & POLLNVAL)) {
                printf("Client %d has disconnected\n", poll_fds[i].fd);
                close(poll_fds[i].fd);
                poll_fds[i].fd = -1;
                //i==0 - сервер вернул событие ошибки/отключения - завершаем программу с ошибкой
                if (i == 0) {
                    printf("server error");
                    exit(1);
                }
                
            }
        }
        //получили запрос на подключение к серверному сокету - пытаемся принять и заносим новый файловый дискриптор в массив поллов.
        if ((poll_fds[0].revents & POLLIN) || (poll_fds[0].revents & POLLPRI)) {
            int cl;
            if ((cl = accept(socket_descriptor, NULL, NULL)) == -1) {
                perror("accept error");
                continue;
            }
            printf("Client %d has connected\n", cl);
            char check_possibility_to_connect = 0;
            for (int i = 1; i < poll_length; i++) {
                if (poll_fds[i].fd < 0) {
                    check_possibility_to_connect = 1;
                    poll_fds[i].fd = cl;
                    poll_fds[i].events = POLLIN | POLLPRI;
                    break;
                }
            }
            if (check_possibility_to_connect == 0)
                perror("poll list overflow");
        }
        // проверяем наличие событий, сообщающих о наличии входных данных с файлового дескриптора.
        for (int i = 1; i < poll_length; i++) {
            if (poll_fds[i].fd < 0) continue;
            int current_client = poll_fds[i].fd;
            if ((poll_fds[i].revents & POLLIN) || (poll_fds[i].revents & POLLPRI)) {
                if ((rc = read(current_client, buffer, 128)) > 0) print_received_data(buffer, current_client, rc);
                if (rc == -1) {
                    perror("read error");
                    exit(-1);
                }
            }
        }
    }
    close(socket_descriptor);
    unlink(socket_path);
    return 0;
}
