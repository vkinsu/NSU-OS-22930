#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc == 1) { return 1; }

    pid_t pid = fork();
    if (pid == -1) { return 1; }

    if (pid == 0) {
        // Child process
        execvp(argv[1], argv + 1);
        return 1;
    } else {
        // Parent process
        int stat_loc;
        wait(&stat_loc);

        if (WIFEXITED(stat_loc)) {
            printf("\nChild process (pid: %d) finished with exit code %d\n",
                   pid, WEXITSTATUS(stat_loc));
        }
    }

    return 0;
}