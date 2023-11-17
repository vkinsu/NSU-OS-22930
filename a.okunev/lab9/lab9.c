
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid;

    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        execlp("cat", "cat", "lines.txt", NULL);
        printf(" CHILD: mine PID -- %d\n", getpid());
        printf(" CHILD: PID mine parent -- %d\n", getppid());
    } else {
        printf("Parent process: Child process is executing cat(1)\n");
        wait(NULL); 
        printf("Parent process: Child process has finished\n");
    }

    return 0;
}