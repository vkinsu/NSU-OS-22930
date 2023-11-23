#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t child_pid;


    child_pid = fork();

    if (child_pid < 0) {

        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {

        // Выполнение команды cat для длинного файла
        execlp("cat", "cat", "longfile.txt", (char *)NULL);

        perror("execlp");
        exit(EXIT_FAILURE);
    } else {

        int status;
        waitpid(child_pid, &status, 0);

        if (WIFEXITED(status)) {
            printf(" Родительский процесс: Завершение дочернего процесса. Последняя строка.\n");
        } else {
            printf("Родительский процесс: Дочерний процесс завершился с ошибкой.\n");
        }
    }

    return 0;
}
