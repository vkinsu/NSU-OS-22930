#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
// пример использования программы: ./program ls -l
int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Использование: %s КОМАНДА [АРГУМЕНТ1 АРГУМЕНТ2 ...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    pid_t child_pid = fork();

    if (child_pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {



        execvp(argv[1], &argv[1]);


        perror("execvp");
        exit(EXIT_FAILURE);
    } else {


        int status;
        waitpid(child_pid, &status, 0);

        if (WIFEXITED(status)) {
            printf("Код завершения порожденного процесса: %d\n", WEXITSTATUS(status));
        } else {
            printf("Дочерний процесс завершился с ошибкой.\n");
        }
    }

    return 0;
}
