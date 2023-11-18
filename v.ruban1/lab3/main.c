#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main() {
    const char *filename = "file.txt";

    // Печать реального и эффективного идентификаторов пользователя
    printf("Real UID: %d\nEffective UID: %d\n", getuid(), geteuid());

    // Открытие файла с помощью fopen
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    } else {
        printf("File opened successfully.\n");
        fclose(file);
    }

    // Сделать, чтобы реальный и эффективный идентификаторы пользователя совпадали
    if (setuid(getuid()) == -1) {
        perror("Error setting UID");
        exit(EXIT_FAILURE);
    }

    // Повторное открытие файла после изменения UID
    file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file after UID change");
        exit(EXIT_FAILURE);
    } else {
        printf("File opened successfully after UID change.\n");
        fclose(file);
    }

    return 0;
}
