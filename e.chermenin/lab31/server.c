#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <strings.h>
#include <ctype.h>
#include <poll.h>

#define BACKLOG (5)
#define POLL_LENGTH (BACKLOG + 1)

char* socket_path = "\0hidden";

int addConnection(struct pollfd* poll_list, int fd) {
  int result = -1;

  for (int i = 1; i < POLL_LENGTH; i++) {
    if (poll_list[i].fd < 0) {
      result = 1;
      poll_list[i].events = POLLIN | POLLPRI;
      poll_list[i].fd = fd;
      result = 0;
      break;
    }
  }

  return result;
}


int main() {
  char buf[100];
  int fd, cl, rc;

  if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
    perror("socket error");
    exit(-1);
  }

  struct sockaddr_un addr;
  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path) - 1);

  unlink(socket_path);

  if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
    perror("bind error");
    exit(-1);
  }

  if (listen(fd, BACKLOG) == -1) {
    perror("listen error");
    exit(-1);
  }

  struct pollfd poll_fds[POLL_LENGTH];
  for (int i = 0; i < POLL_LENGTH; i++) {
    poll_fds[i].fd = -1;
    poll_fds[i].events = POLLIN | POLLPRI;

    poll_fds[0].fd = fd;

    while (1) {
      if ((poll(poll_fds, POLL_LENGTH, -1)) == -1) {
        perror("bad poll");
        exit(-1);
      }

      for (int i = 0; i < POLL_LENGTH; i++) {
        if (poll_fds[i].fd < 0) continue;
        short revents = poll_fds[i].revents;

        if ((revents & POLLERR) || (revents & POLLHUP) || (revents & POLLNVAL)) {
          close(poll_fds[i].fd);
          poll_fds[i].fd = -1;

          if (i == 0) {
            printf("Server error");
            exit(-1);
          }
          else {
            printf("Closing socket\n");
          }
        }
      }

      if ((poll_fds[0].revents & POLLIN) || (poll_fds[0].revents & POLLPRI)) {
        if ((cl = accept(fd, NULL, NULL)) == -1) {
          perror("accept error");
          continue;
        }

        if (addConnection(poll_fds, cl) == -1) {
          perror("Failed to add new connection");
        }
      }
      for (int i = 1; i < POLL_LENGTH; i++) {
        if (poll_fds[i].fd < 0) continue;
        int cur_desc = poll_fds[i].fd;

        if ((poll_fds[i].revents & POLLIN) || (poll_fds[i].revents & POLLPRI)) {
          if ((rc = read(cur_desc, buf, sizeof(buf))) > 0) {
            for (int j = 0; j < rc; j++) {
              buf[j] = toupper(buf[j]);
              printf("%c", buf[j]);
            }
          }

          if (rc == -1) {
            perror("read");
            exit(-1);
          }
          else if (rc == 0) {
            printf("EOF reached, closing connection\n");
            close(cur_desc);
            poll_fds[i].fd = -1;
          }
        }
      }
    }
  }
  return 0;
}