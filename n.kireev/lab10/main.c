#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
    int status;

    pid_t sub_proc = fork();
    if (sub_proc == -1)
    {
        perror("Failed sub-processing");
        _exit(1);
    }
    char * path = (char*)malloc(sizeof(char)*(strlen(argv[1]) + 10));
    strcpy(path, "/bin/");
    strcat(path, argv[1]);

    if (sub_proc == 0)       // 0, if we in sub-process
    {
        execv(path, &argv[1]);
        printf("if all good, you not able to see it in consol\n");
        exit(errno);
    }
    else
    {
        if (waitpid(sub_proc, &status, WUNTRACED) != sub_proc)      // wait for ending sub-process
        {
            perror("Don't know, another process shut down");
            exit(7);
        }
        else
        {
            printf("\nreturn value of program: %d\n", WEXITSTATUS(status));
        }
    }
    free(path);
}
