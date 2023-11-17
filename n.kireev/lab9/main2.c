#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    printf("\nid_main: %d\nid_par_main: %d\n\n", getpid(), getppid());
    pid_t sub_proc = fork();
    printf("\nid_1: %d -- id_2: %d -- sub_proc: %d\n\n", getpid(), getppid(), sub_proc);
    int status;

    if (sub_proc == -1)
    {
        perror("Failed sub-processing");
        _exit(1);
    }

    if (sub_proc == 0)       // 0, if we in sub-process
    {
        execl("/bin/cat", "cat", "some.txt", NULL);
        _exit(EXIT_FAILURE);
    }
    else
    {
        if (waitpid (sub_proc, &status, WUNTRACED) != sub_proc)      // wait for ending sub-process
        {
            perror("Don't know, another process shut down");
            _exit(1);
        }
        else
        {
            printf("\n\nsilence, when parent is talking\n");
        }
    }
}
