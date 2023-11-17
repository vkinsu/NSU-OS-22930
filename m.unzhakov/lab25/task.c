#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <ctype.h>
#include <sys/syscall.h> 
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <wait.h>
#include <fcntl.h>


int pipe_filedescs[2];

extern int errno;

int main() {
    pid_t child_proc, child_dummy;
    int status;
    
    if (pipe(pipe_filedescs) != 0)
        printf("can't create a programm channel with error code: %d\n", errno);

    printf("original pid of process: %d\n", getpid());
    
    child_proc = fork();
    if (child_proc == (pid_t)-1) {
        printf("error with process creating: %s\n", strerror(status));
        exit(1);
    }
    if (!child_proc) {
        close(pipe_filedescs[0]);
        int file_des = open("text_with_dif_regs", O_RDONLY);
        char x;
        while (read(file_des, &x, 1) != 0)
            write(pipe_filedescs[1], &x, 1);
        close(pipe_filedescs[1]);
        exit(0);
    }
    else {
        close(pipe_filedescs[1]);
        char buf = 0;
        int length;
        while (length = read(pipe_filedescs[0], &buf, 1) != 0)
            printf("%c", (char)toupper(buf));
        close(pipe_filedescs[0]);
    }

    do {
        child_dummy = waitpid(child_proc, &status, 0);
        if (child_dummy == (pid_t)-1 && errno != EINTR)
            break;
    } while(child_dummy != child_proc);
    return 0;
}