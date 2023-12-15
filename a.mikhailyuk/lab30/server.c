#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#define SERVER "./qwerty123456"

void print_upper(char* buf)
{
    int i = 0;
    while (buf[i] != '\0')
    {
        char c = putchar(toupper(buf[i]));
        i++;
        printf("%c", c);
    }
}

int main()
{
    unlink(SERVER);
    int sock, listener;
    struct sockaddr_un addr;
    char buf[1024];
    int bytes_read;

    listener = socket(AF_UNIX, SOCK_STREAM, 0);
    if (listener < 0)
    {
        perror("Socket creation error");
        exit(1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SERVER);
    if (bind(listener, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        perror("Binding of listener error");
        exit(2);
    }

    if (listen(listener, 7) < 0)
    {
        perror("Listening error");
        exit(3);
    }

    int flag = 0;
    while (1)
    {
        sock = accept(listener, NULL, NULL);

        if (sock < 0)
        {
            perror("Acception error");
            continue;
        }

        while(1)
        {
            bytes_read = recv(sock, buf, 1024, 0);
            if (bytes_read <= 0)
            {
                flag = 1;
                break;
            }
            int i = 0;
            while (buf[i] != '\0')
            {
                putchar(toupper(buf[i]));
                i++;
            }
            //putchar('\n');
        }

        close(sock);

        if (flag)
        {
            break;
        }
    }
    close(listener);
    unlink(SERVER);
    return 0;
}