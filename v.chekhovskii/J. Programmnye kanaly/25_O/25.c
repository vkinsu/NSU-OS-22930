#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>

main(int argc, char *argv[]) {
    if (argc != 2) {
        perror("Please provide a string you want to convert to uppercase.\n");
        return 2;
    }

    int pipe_fd[2]; // 0 -- reading fd, 1 -- writing fd
    if (pipe(pipe_fd) == (pid_t)-1) {
        perror("Unable to create pipe.\n");
        return -1;
    }

    pid_t child = fork();
    if (child == (pid_t)-1) {
        perror("Unable to create child process.\n");
        return -1;
    }

    else if (child == (pid_t)0) { // child process
        //printf("Started uppercase conversion in child process...\n");
        close(pipe_fd[1]);

        char lineBuffer[128];
        read(pipe_fd[0], lineBuffer, 128); // read from reading fd


        for (ssize_t i = 0; i < strlen(lineBuffer); i++) {
            lineBuffer[i] = toupper(lineBuffer[i]);
        }

        write(STDOUT_FILENO, lineBuffer, strlen(lineBuffer));
        printf("\n");

        close(pipe_fd[0]);
    }

    else { // parent process
        close(pipe_fd[0]);

        write(pipe_fd[1], argv[1], strlen(argv[1])); // write to writing fd

        close(pipe_fd[1]);

        waitpid(child, NULL, 0);
    }

    return 0;
}