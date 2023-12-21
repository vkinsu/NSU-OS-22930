#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/wait.h>

int main() {
    pid_t child_proc, child_dummy;
    int status;
    int pipe_filedescs[2];

    if (pipe(pipe_filedescs) != 0) {
        perror("Failed to create a pipe");
        exit(EXIT_FAILURE);
    }

    printf("original pid of process: %d\n", getpid());
    
    child_proc = fork();
    if (child_proc == (pid_t)-1) {
        perror("Failed to fork a process");
        exit(EXIT_FAILURE);
    }
    if (!child_proc) {
        close(pipe_filedescs[0]);
        int file_des = open("text_with_dif_regs", O_RDONLY);
        if (file_des < 0) {
            perror("Failed to open file");
            close(pipe_filedescs[1]);
            exit(EXIT_FAILURE);
        }

        // Use a buffer rather than individual characters to boost IO performance
        char buffer[4096];
        ssize_t nread;
        while ((nread = read(file_des, buffer, sizeof(buffer))) > 0) {
            // Convert to upper case before writing to the pipe
            for (ssize_t i = 0; i < nread; ++i) {
                buffer[i] = toupper(buffer[i]);
            }
            write(pipe_filedescs[1], buffer, nread);
        }
        close(file_des);
        close(pipe_filedescs[1]);
        exit(EXIT_SUCCESS);
    } else {
        close(pipe_filedescs[1]);
        char buffer[4096];
        ssize_t nread;
        // Read the data from the pipe
        while ((nread = read(pipe_filedescs[0], buffer, sizeof(buffer))) > 0) {
            write(STDOUT_FILENO, buffer, nread);
        }
        close(pipe_filedescs[0]);
    }

    child_dummy = waitpid(child_proc, &status, 0);
    if (child_dummy == (pid_t)-1) {
        perror("waitpid failed");
        exit(EXIT_FAILURE);
    }
    if (WIFEXITED(status)) {
        printf("Child exited with status %d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("Child killed by signal %d\n", WTERMSIG(status));
    }
    
    return EXIT_SUCCESS;
}
