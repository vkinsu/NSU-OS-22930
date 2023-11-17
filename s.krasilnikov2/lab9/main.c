#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        char* args[] = {"cat", "test.txt", NULL};
        execvp("cat", args);

        perror("execvp");
        exit(EXIT_FAILURE);
    } else {
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            printf("Parent process: Child process exited with status %d.\n", WEXITSTATUS(status));
        }
        else {
            printf("Parent process: Child process exited with error.\n");
        }
        printf("Parent process: This line is printed after the child process.\n");
    }

    return 0;
}
