#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main() {
    uid_t ruid, euid;
    FILE *fp;

    fp = fopen("file", "w+");
    if (fp == NULL) {
        perror("Ошибка при открытии файла");
        exit(EXIT_FAILURE);
    }

    ruid = getuid();
    euid = geteuid();
    printf("Реальный идентификатор пользователя: %d\n", ruid);
    printf("Эффективный идентификатор пользователя: %d\n", euid);

    if (setuid(geteuid()) == -1) {
        perror("Ошибка при установке идентификатора пользователя");
        exit(EXIT_FAILURE);
    }

    ruid = getuid();
    euid = geteuid();
    printf("Реальный идентификатор пользователя: %d\n", ruid);
    printf("Эффективный идентификатор пользователя: %d\n", euid);

    fclose(fp);

    return 0;
}
