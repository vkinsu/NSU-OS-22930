#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
int socket_descriptor = -1;
typedef struct server{
    struct sockaddr addr;
    socklen_t addrlen;
    int socket_descriptor;
}server;

char *socket_path = "\0hidden";

void close_prog(int) {
    char exit_sig = '\007';
    write(socket_descriptor, &exit_sig, 1);
    close(socket_descriptor);
    exit(0);
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        socket_path = argv[1];
    }
    signal(SIGINT, close_prog);
    printf("if you want to close app just write 'exit' :)\n");
    socket_descriptor = socket(AF_UNIX, SOCK_STREAM, 0);
    int rc;
    struct sockaddr_un addr;

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    
    if (*socket_path == '\0') {
        *addr.sun_path = '\0';
        strncpy(addr.sun_path+1, socket_path+1, sizeof(addr.sun_path)-2);
    } else {
        strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path)-1);
    }

    if (connect(socket_descriptor, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("connect error");
        exit(-1);
    }
    char buf[100];
    while((rc = strlen(fgets(buf, 100, stdin))) > 0) {
        if (strcmp(buf, "exit\n") == 0) {
            char exit_sig = '\007';
            write(socket_descriptor, &exit_sig, 1);
            close(socket_descriptor);
            break;
        }
        printf("size of message: %d\n", rc);
        if (write(socket_descriptor, buf, rc) != rc) {
            if (rc > 0) fprintf(stderr,"partial write");
            else {
                perror("write error");
                exit(-1);
            }
        }
    }
    return 0;
}