#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#define BUFFER_SIZE 256

int main() {
    int pipe_fd[2];
    pid_t child_pid;

    // Создание программного канала
    if (pipe(pipe_fd) == -1) {
        perror("Error creating pipe");
        exit(EXIT_FAILURE);
    }

    // Создание подпроцесса
    if ((child_pid = fork()) == -1) {
        perror("Error creating child process");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {
        // Код, выполняемый в дочернем процессе (читатель из канала)
        close(pipe_fd[1]); // Закрываем записывающий конец

        char buffer[BUFFER_SIZE];
        ssize_t bytes_read;

        while ((bytes_read = read(pipe_fd[0], buffer, BUFFER_SIZE)) > 0) {
            for (ssize_t i = 0; i < bytes_read; i++) {
                buffer[i] = toupper(buffer[i]);
            }

            write(STDOUT_FILENO, buffer, bytes_read);
        }

        close(pipe_fd[0]);
        exit(EXIT_SUCCESS);
    } else {
        // Код, выполняемый в родительском процессе (писатель в канал)
        close(pipe_fd[0]); // Закрываем читающий конец

        const char *message = "Hello, world!\n";

        write(pipe_fd[1], message, strlen(message));

        close(pipe_fd[1]); // Закрываем записывающий конец

        waitpid(child_pid, NULL, 0); // Ждем завершения дочернего процесса
    }

    return 0;
}
