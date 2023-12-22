#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

int main() {
  int pipe_fd[2];
  pid_t child_pid;
  pipe(pipe_fd);
  child_pid = fork();
  if (child_pid == 0) {
    close(pipe_fd[1]);
    char buffer[100];
    ssize_t bytes_read = read(pipe_fd[0], buffer, sizeof(buffer));
    close(pipe_fd[0]);
    for (ssize_t i = 0; i < bytes_read; ++i) {
      buffer[i] = toupper(buffer[i]);
    }
    write(STDOUT_FILENO, buffer, bytes_read);
    exit(EXIT_SUCCESS);
  } else {
    close(pipe_fd[0]);
    char text[] = "Some TEXT foR TeSt.\n\n";
    write(pipe_fd[1], text, sizeof(text));
    close(pipe_fd[1]);
    wait(NULL);
  }
  return 0;
}