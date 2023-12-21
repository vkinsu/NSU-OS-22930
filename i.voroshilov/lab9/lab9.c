#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main() {
    pid_t pid = fork();
    if (pid == -1) { return 1; }

    if (pid == 0) {
        // Child process
        execlp("cat", "cat", "../task1.c", NULL);
        return 1;
    } else {
        // Parent process
        if(wait(NULL) != -1) {
            printf("\nChild process (pid: %d) finished\n", pid);
        }
    }

    return 0;
}