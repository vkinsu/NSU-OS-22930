#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <aio.h>
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <string.h>
#include <ctype.h>
#include <sys/un.h>
#include <vector>
#define MAX_PACKAGE_SIZE 1024

struct sigevent my_sigev;
int socket_descriptor = -1;
std::vector<aiocb*> all_clients_aiocbs;

int print_received_data(int sig, siginfo_t* info, void* ucontext) {
    std::vector<aiocb*>* clients = (std::vector<aiocb*>*)info->si_value.sival_ptr;
    for (int i = 0; i < all_clients_aiocbs.size(); i++) {
        if (aio_return((*clients)[i]) > 0) {
            if (*(char*)(*clients)[i]->aio_buf == '\0') continue;
            if (*(char*)(*clients)[i]->aio_buf == '\007') {
                printf("Client with descriptor %d has disconnected\n", (*clients)[i]->aio_fildes);
                aio_cancel((*clients)[i]->aio_fildes, (*clients)[i]);
                char* buffer = (char*)(*clients)[i]->aio_buf;
                memset(buffer, 0, sizeof(strlen(buffer)));
                close((*clients)[i]->aio_fildes);
                (*clients)[i]->aio_fildes = -1;
                return 1;
            }

            printf("Client with descriptor %d has sent a message: ", (*clients)[i]->aio_fildes);
            char* buffer = (char*)(*clients)[i]->aio_buf;
            int j = 0;
            for (; buffer[j] != '\0'; j++)
                printf("%c", toupper(buffer[j]));
            printf("with size %d\n", j);
            memset(buffer, 0, sizeof(j));
            aio_read((*clients)[i]);
        }
    }
    return 1;
}

aiocb* create_new_aiocb(int client_descriptor) {
    aiocb* new_aiocb = (struct aiocb*)malloc(sizeof(struct aiocb));
    new_aiocb->aio_buf = calloc(MAX_PACKAGE_SIZE, 1);
    new_aiocb->aio_fildes = client_descriptor;
    new_aiocb->aio_nbytes = MAX_PACKAGE_SIZE;
    new_aiocb->aio_offset = 0;
    new_aiocb->aio_sigevent = my_sigev;
    return new_aiocb;
}

char *socket_path = "\0hidden";

void close_prog(int) {
    for (int i = 0; i < all_clients_aiocbs.size(); i++) 
        aio_cancel(all_clients_aiocbs[i]->aio_fildes, all_clients_aiocbs[i]);
    
    close(socket_descriptor);
    unlink(socket_path);
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc > 1) socket_path=argv[1];
    signal(SIGINT, close_prog);
    struct sigaction sa;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = (void(*)(int, siginfo_t*, void*))print_received_data;
	sigaction(SIGUSR1, &sa, NULL);

    socket_descriptor = socket(AF_UNIX, SOCK_STREAM, 0);
    if (socket_descriptor < 0)
        perror("returned error while socket was starting up");
    
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
	my_sigev.sigev_notify = SIGEV_SIGNAL;
	my_sigev.sigev_signo = SIGUSR1;
	my_sigev.sigev_value.sival_ptr = &all_clients_aiocbs;
    while (1) {
        int client_descriptor = accept(socket_descriptor, 0 , 0);
        if (client_descriptor < 0) continue;
        bool find_a_slot = 0;
        for (int i = 0; i < all_clients_aiocbs.size(); i++) {
            if (all_clients_aiocbs[i]->aio_fildes == -1) {
                all_clients_aiocbs[i]->aio_fildes = client_descriptor;
                aio_read(all_clients_aiocbs[i]);
                find_a_slot = 1;
            }
        }
        if (!find_a_slot) {
            all_clients_aiocbs.push_back(create_new_aiocb(client_descriptor));
            aio_read(all_clients_aiocbs[all_clients_aiocbs.size() - 1]);
        }
        printf("Client %d has connected\n", client_descriptor);
    }
    return 0;
}