#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <sys/un.h> 
#include <string.h> 
 
#define MAX_CLIENTS 10 
 
struct client_info { 
    int sockfd; 
    // Дополнительные данные о клиенте могут быть добавлены здесь 
}; 

char myToupper(char a) {
    if (a >= 97) {
        return a - 32;
    }
    else return a;
}
 
int main() { 
    unlink("/tmp/socket");
    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        perror("socket"); 
        exit(EXIT_FAILURE); 
    } 
 
    struct sockaddr_un addr; 
    addr.sun_family = AF_UNIX; 
    strncpy(addr.sun_path, "/tmp/socket", sizeof(addr.sun_path) - 1); 
 
    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) { 
        perror("bind"); 
        close(sockfd); 
        exit(EXIT_FAILURE); 
    } 
 
    listen(sockfd, SOMAXCONN); 
 
    printf("Сервер ожидает подключения...\n"); 
 
    struct client_info clients[MAX_CLIENTS]; 
    fd_set read_fds, active_fds; 
 
    FD_ZERO(&active_fds); 
    FD_SET(sockfd, &active_fds); 

    while (1) { 
        printf("1\n");
        read_fds = active_fds; 
 
        if (select(FD_SETSIZE, &read_fds, NULL, NULL, NULL) == -1) { 
            perror("select"); 
            break; 
        } 
 
        for (int fd = 0; fd < FD_SETSIZE; fd++) { 
            printf("2");
            if (FD_ISSET(fd, &read_fds)) { 
                if (fd == sockfd) { 
                    // Новое подключение 
                    int client_sockfd = accept(sockfd, NULL, NULL); 
                    if (client_sockfd == -1) { 
                        perror("accept"); 
                        break; 
                    } 

                    printf("Клиент подключен (fd=%d)!\n", client_sockfd); 
 
                    FD_SET(client_sockfd, &active_fds); 
 
                    // Добавить данные о клиенте в массив clients 
                    // (здесь может потребоваться использование динамического выделения памяти) 
 
                    // Пример: 
                    int i; 
                    for (i = 0; i < MAX_CLIENTS; ++i) { 
                        if (clients[i].sockfd == 0) { 
                            clients[i].sockfd = client_sockfd; 
                            break; 
                        } 
                    } 
 
                    if (i == MAX_CLIENTS) { 
                        fprintf(stderr, "Превышено максимальное количество клиентов.\n"); 
                        close(client_sockfd); 
                    } 
                } 
                else { 
                    // Данные от клиента 
                    char buffer[1024]; 
                    int bytes_received = recv(fd, buffer, sizeof(buffer), 0); 
                    if (bytes_received <= 0) { 
                        // Разрыв соединения или ошибка 
                        printf("Клиент отключен (fd=%d)!\n", fd); 
                        close(fd); 
                        FD_CLR(fd, &active_fds); 
 
                        // Удалить данные о клиенте из массива clients 
                        // (может потребоваться дополнительная логика) 
                        for (int i = 0; i < MAX_CLIENTS; ++i) { 
                            if (clients[i].sockfd == fd) { 
                                clients[i].sockfd = 0; 
                                break; 
                            } 
                        } 
                    } else { 
                        // Обработка данных от клиента (пример) 
                        buffer[bytes_received] = '\0'; 
                        printf("Получены данные от клиента (fd=%d): %s\n", fd, buffer); 
                    } 
                } 
            } 
        } 
    } 
 
    // Закрытие всех активных сокетов 
    for (int i = 0; i < MAX_CLIENTS; ++i) { 
        if (clients[i].sockfd != 0) { 
            close(clients[i].sockfd); 
        } 
    } 
 
    // Закрытие сокета сервера 
    close(sockfd); 
    unlink("/tmp/socket");
 
    return 0; 
}