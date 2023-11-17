#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include "string.h"
#include <sys/wait.h>
int main() {
    int pipe_fd[2];
    pid_t child_pid;

    if (pipe(pipe_fd) == -1) {
        perror("Error creating pipe");
        exit(EXIT_FAILURE);
    }

    child_pid = fork();

    if (child_pid == -1) {
        perror("Error creating child process");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {
        close(pipe_fd[1]);

        char buffer[256];
        ssize_t bytes_read;

        while ((bytes_read = read(pipe_fd[0], buffer, sizeof(buffer))) > 0) {
            for (ssize_t i = 0; i < bytes_read; ++i) {
                buffer[i] = toupper(buffer[i]);
            }

            write(STDOUT_FILENO, buffer, bytes_read);
        }

        close(pipe_fd[0]);
        exit(EXIT_SUCCESS);
    } else {
        close(pipe_fd[0]);

        const char* message = "Hello, World!\n";
        write(pipe_fd[1], message, strlen(message));  // Пишем в канал

        close(pipe_fd[1]);
        wait(NULL);
        exit(EXIT_SUCCESS);
    }

    return 0;
}
