#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <spawn.h>
#include <errno.h>
#include <string.h>


int main(int argc, char* argv[]) {
    pid_t chlp, chld;
    char *argchl[] = {argv[1], argv[2], NULL};
    int status;

    chlp = fork();

    if (chlp == (pid_t)-1) {
        printf("error with process creating\n");
        return 1;
    }
    if (!chlp) {
        printf("process has been created with pid: %i\n", getpid());
        char location[50] = "/bin/";
        strcat(location, argchl[0]);
        exit(execv(location, argchl));
    }
    do {
        chld = waitpid(chlp, &status, 0);
        if (chld == (pid_t)-1 && errno != EINTR)
            break;
    } while(chld != chlp);
    printf("exit status was %d\n", WEXITSTATUS(status));
    return 0;
}