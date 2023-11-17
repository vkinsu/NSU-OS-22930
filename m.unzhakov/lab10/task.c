#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <spawn.h>
#include <errno.h>
#include <string.h>


int main(int argc, char* argv[]) {
    pid_t child_proc, child_dummy;
    char *args_to_child[] = {argv[1], argv[2], NULL};
    int status;

    child_proc = fork();

    if (child_proc == (pid_t)-1) {
        printf("error with process creating\n");
        return 1;
    }
    if (!child_proc) {
        printf("process has been created with pid: %i\n", getpid());
        char location[50] = "/bin/";
        strcat(location, args_to_child[0]);
        exit(execv(location, args_to_child));
    }
    do {
        child_dummy = waitpid(child_proc, &status, 0);
        if (child_dummy == (pid_t)-1 && errno != EINTR)
            break;
    } while(child_dummy != child_proc);
    printf("exit status was %d\n", WEXITSTATUS(status));
    return 0;
}