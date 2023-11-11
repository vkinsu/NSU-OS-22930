
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int execute_command(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Invalid argument number\n");
        return 1;
    }
    
    pid_t pid = fork();
    int return_value = 0;
    
    if (pid == 0) {
        execvp(argv[1], &argv[1]);
        fprintf(stderr, "Wrong command name\n");
        exit(-1);
    } else if (pid == -1) {
        fprintf(stderr, "Can't fork\n");
    }
    
    if (wait(&return_value) == -1) {
        perror("wait");
        return -1;
    }
    
    printf("\nReturn Value = %d\n", WEXITSTATUS(return_value));
    return 0;
}

int main(int argc, char **argv) {
    return execute_command(argc, argv);
}

