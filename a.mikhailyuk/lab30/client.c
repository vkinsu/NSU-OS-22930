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
#define SERVER "./qwerty123456"

char message[] = "Glaub inda nordri\nCauti. Innigan\n";
char buf[sizeof(message)];

int main()
{
    int sock;
    struct sockaddr_un addr;

    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if(sock < 0)
    {
        perror("Socket creation error");
        exit(1);
    }
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SERVER);
    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("Connection error");
        exit(2);
    }
    printf("enter '~' to exit\n");
    char buf[1024];
    fgets(buf, 1024, stdin);
    while(!(buf[0] == '~' && (buf[1] == '\0' || buf[1] == '\n')))
    {
        send(sock, buf, sizeof(buf), 0);
        fgets(buf, 1024, stdin);
    }
    close(sock);
    return 0;
}