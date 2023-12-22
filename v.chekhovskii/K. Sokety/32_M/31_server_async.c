#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/epoll.h>
#include <ctype.h>

int set_nonblocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    return 0;
}

int main() {
    int server_fd, client_fd, epoll_fd;
    struct sockaddr_un server_addr, client_addr;
    struct epoll_event event, events[10];

    // создание структуры сокета (серверного)
    if ((server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation error");
        exit(1);
    }

    // создание сокета (серверного)
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, "/tmp/32_server");

    unlink("/tmp/32_server"); // на всякий случай
    // связываем серверный сокет с адресом
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Socket binding error");
        exit(1);
    }

    // установка сокета в неблокирующий режим
    // (нужно для асинхронной обработки событий)
    if (set_nonblocking(server_fd) == -1) {
        perror("Unable to set socket in non-blocking mode");
        exit(1);
    }

    // перевод серверного сокета в режим прослушивания
    if (listen(server_fd, SOMAXCONN) == -1) {
        perror("Unable to fetch information from client");
        exit(2);
    }
    else {
        printf("SERVER UP & LISTENING.\n\n");
    }

    // создание epoll-дескриптора
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("Error from epoll_create1");
        exit(2);
    }

    // добавление серверного сокета в epoll
    event.events = EPOLLIN;
    event.data.fd = server_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event) == -1) {
        perror("Error from epoll_ctl");
        exit(2);
    }

    char buffer[256];
    ssize_t valread;
    while (1) { // обработка сообщений (получение)

        // ожидание событий от epoll
        int nfds = epoll_wait(epoll_fd, events, 10, -1);
        if (nfds == -1) {
            perror("Error from epoll_wait");
            exit(2);
        }

        // обработка событий
        for (int i = 0; i < nfds; ++i) {
            if (events[i].data.fd == server_fd) {
                // обработка нового подключения от клиента
                socklen_t client_len = sizeof(client_addr);
                if ((client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len)) == -1) {
                    perror("Unable to accept connection from client");
                    exit(2);
                }

                if (set_nonblocking(client_fd) == -1) {
                    printf("Error: Client is not set in non-blocking mode\n");
                    exit(2);
                }
                // добавление клиентского сокета в мониторинг epoll
                event.events = EPOLLIN | EPOLLET;
                event.data.fd = client_fd;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event) == -1) {
                    perror("Error from epoll_ctl");
                    exit(1);
                }

                printf("[+] New client joined.\n");
            } else {
                // обработка данных от клиента
                if ((valread = read(events[i].data.fd, buffer, sizeof(buffer))) == 0) {
                    close(events[i].data.fd);
                    printf("[-] The client left.\n");
                } else if (valread > 0) {
                    int j = 0;
                    for (j; j < valread; j++) {
                        buffer[j] = toupper(buffer[j]);
                    }
                    buffer[j] = '\0';
                    printf("New message from client!\n%s\n", buffer);
                } else {
                    printf("Erorr: Unable to read message from client\n");
                }
            }
        }
    }

    // закрытие сокета (серверного) и удаление файла сокета
    close(server_fd);
    unlink("/tmp/32_server");
    printf("\nSERVER JOB IS DONE.\n");
    return 0;
}
