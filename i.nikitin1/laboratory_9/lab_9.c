#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>

int main(int argc, char* argv[]) {

    assert(argc<=2);
    pid_t proc_id;

    proc_id = fork();

    if (proc_id < 0) {
        perror("fork");
        exit(1);
    } else if (proc_id == 0) {
        execlp("cat", "cat", argv[1], NULL);
        printf("Новый процесс собственный ID -- %d\n", getpid());
        printf("Новый процесс ID процесса-родителя-- %d\n", getppid());
    } else {
        printf("Процесс родителя был вызван cat(1) новым процессом\n");
        wait(NULL); 
        printf("Процесс родителя - новый процесс завершен!\n");
    }

    return 0;
}