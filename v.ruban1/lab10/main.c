#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command> [arg1] [arg2] [...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    pid_t child_pid = fork();

    if (child_pid == -1) {
        perror("Error creating child process");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {
        // Код, выполняемый в порожденном процессе
        execvp(argv[1], &argv[1]);  // Заменяем текущий образ процесса на указанную команду с аргументами
        perror("Error executing command");
        exit(EXIT_FAILURE);
    } else {
        // Код, выполняемый в родительском процессе
        int status;
        waitpid(child_pid, &status, 0);

        if (WIFEXITED(status)) {
            printf("Child process exited with status %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Child process terminated by signal %d\n", WTERMSIG(status));
        } else {
            printf("Child process didn't exit normally\n");
        }
    }

    return 0;
}
