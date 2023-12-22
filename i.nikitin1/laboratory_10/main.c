#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char *argv[]) {
    // Проверяем, переданы ли аргументы командной строки
    if (argc < 2) {
        fprintf(stderr, "Отсутствуют аргументы для передачи процесса!\n");
        exit(EXIT_FAILURE);
    }

    // Получаем команду из аргументов командной строки
    char *command_to_run = argv[1];

    // Получаем оставшиеся аргументы (аргументы для передачи команде)
    char *arguments_to_pass[argc];
    for (int i = 1; i < argc; i++) {
        arguments_to_pass[i - 1] = argv[i];
    }
    arguments_to_pass[argc - 1] = NULL; 
    
    pid_t pid;
    int status;

    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Этот код выполняется в дочернем процессе
        execvp(command_to_run, arguments_to_pass);
        perror("execvp");
        exit(EXIT_FAILURE);
    } else {
        // Этот код выполняется в родительском процессе
        // Ждем завершения дочернего процесса
        waitpid(pid, &status, 0);
        printf("Код завершения процесса '%s': %d\n", command_to_run, WEXITSTATUS(status));
    }

    return 0;
}