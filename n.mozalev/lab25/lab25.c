#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

int main() {
    int fd[2];
    pid_t pid;

    if (pipe(fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) { 
        close(fd[0]);

        char text[] = "Hello, World!";
        write(fd[1], text, sizeof(text)); 

        close(fd[1]);
    } else {
        close(fd[1]);

        char text[100];
        read(fd[0], text, sizeof(text));

        for (int i = 0; text[i] != '\0'; i++) {
            text[i] = toupper(text[i]);
        }

        printf("%s\n", text);
        
        close(fd[0]);
    }

    return 0;
}

