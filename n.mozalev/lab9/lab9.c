#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void childProcess() {
    system("cat long_file.txt");
}

void parentProcess() {
    printf("Hello, world\n");
}

int main(int argc, char * argv[]) {
    int rv = 0;
    pid_t pid = fork();
    
    if (pid == -1) {
        fprintf(stderr, "Error creating child process\n");
        return 1;
    }
    else if (pid == 0) {
        childProcess();
    }
    else {
        parentProcess();
    }
    
    pid = fork();
    
    if (pid == -1) {
        fprintf(stderr, "Error creating child process\n");
        return 1;
    }
    else if (pid == 0) {
        childProcess();
        exit(rv);
    }
    else {
        wait(&rv);
        printf("Child process returned - %d\n", WEXITSTATUS(rv));
    }
    
    return 0;
}

