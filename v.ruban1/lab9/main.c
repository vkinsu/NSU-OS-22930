#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t child_pid;

    // Создание подпроцесса
    if ((child_pid = fork()) == -1) {
        perror("Error creating child process");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {
        // Код, выполняемый в подпроцессе
        execlp("cat", "cat", "longfile.txt", NULL);
        perror("Error executing cat");
        exit(EXIT_FAILURE);
    } else {
        // Код, выполняемый в родительском процессе
        printf("Parent process: Before waiting\n");

        int status;
        waitpid(child_pid, &status, 0);

        if (WIFEXITED(status)) {
            printf("Parent process: After waiting. Child process exited with status %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Parent process: After waiting. Child process terminated by signal %d\n", WTERMSIG(status));
        } else {
            printf("Parent process: After waiting. Child process didn't exit normally\n");
        }
    }

    return 0;
}
