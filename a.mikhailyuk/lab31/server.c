#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define SERVER "./qwerty123456"

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

    fd_set read_fds;
    int max_fd;

    while (1) {
        FD_ZERO(&read_fds);
        FD_SET(listener, &read_fds);
        max_fd = listener;

        if (select(max_fd + 1, &read_fds, 0, 0, 0) < 0)
        {
            perror("Selection error");
            exit(4);
        }

        if (FD_ISSET(listener, &read_fds))
        {
            sock = accept(listener, 0, 0);
            if (sock < 0)
            {
                perror("Acception error");
                continue;
            }

            FD_SET(sock, &read_fds);
            if (sock > max_fd) {
                max_fd = sock;
            }
        }

        for (int now = listener + 1; now <= max_fd; ++now)
        {
            if (FD_ISSET(now, &read_fds))
            {
                while (1)
                {
                    bytes_read = recv(now, buf, 1024, 0);
                    if (bytes_read <= 0)
                    {
                        break;
                    }
                    int i = 0;
                    while (buf[i] != '\0')
                    {
                        putchar(toupper(buf[i]));
                        i++;
                    }
                }
            }
        }
    }

    //  Server forever, because no one said, that I need to crush it
    return 0;
}