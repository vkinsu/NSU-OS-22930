#include <sys/socket.h> 
#include <sys/un.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <strings.h> 
 
char socket_path[] = "/tmp/socket"; 

int main() { 
    char buf[100];  // Буфер для чтения данных 
 
    int fd, rc; 
 
    // Создание сокета 
    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) { 
        perror("socket error");  // Вывод ошибки, если создание сокета не удалось 
        exit(-1); 
    } 
 
    // Заполнение структуры sockaddr_un для адреса UNIX-сокета 
    struct sockaddr_un addr; 
    memset(&addr, 0, sizeof(addr)); 
    addr.sun_family = AF_UNIX; 
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path) - 1); 
 
    // Установка соединения с сервером по заданному адресу 
    if (connect(fd, (struct sockaddr *) &addr, sizeof(addr)) == -1) { 
        perror("connect error");  // Вывод ошибки, если соединение не удалось 
        exit(-1); 
    } 
 
    // Чтение данных с stdin и отправка их по сокету 
    while ((rc = read(STDIN_FILENO, buf, sizeof(buf))) > 0) { 
        if (write(fd, buf, rc) != rc) { 
            if (rc > 0) { 
                fprintf(stderr, "partial write");  // Вывод предупреждения о частичной записи данных 
            } else { 
                perror("write error");  // Вывод ошибки, если запись данных не удалась 
                exit(-1); 
            } 
        } 
    } 
 
    return 0; 
}