#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>




typedef struct client{
  struct sockaddr addr;
  socklen_t addrlen;
  int socket_descriptor;
}client;

char *socket_path = "\0hidden";

int main(int argc, char *argv[]) {
  if (argc > 1) socket_path=argv[1];
  int socket_descriptor = socket(AF_UNIX, SOCK_STREAM, 0);

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
  client cl;
  while (1) {
    struct sockaddr addr;
    socklen_t addrlen;
    int client_descriptor = accept(socket_descriptor, &addr, &addrlen);
    printf("Client with descriptor %d has connected!\n", client_descriptor);
    cl.addr = addr;
    cl.addrlen = addrlen;
    cl.socket_descriptor = client_descriptor;
    break;
  }
  char buffer[128];
  int rc;
  while (1) {
    if ((rc = recv(cl.socket_descriptor, buffer, 128, 0)) > 0) {
      if (*buffer == '\007')
        break;
      printf("size of message: %d\n", rc);
      for (int i = 0; i < rc; i++)
        printf("%c", toupper(buffer[i]));
    }
  }
  close(socket_descriptor);
  unlink(socket_path);
  return 0;
}