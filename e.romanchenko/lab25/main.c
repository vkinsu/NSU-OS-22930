#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main() {
    // Текст для отправки в программный канал
    const char *text = "Hello, World!";

    // Создаем программный канал с помощью popen
    FILE *pipe = popen("tr '[:lower:]' '[:upper:]'", "w");
    if (!pipe) {
        perror("popen");
        exit(EXIT_FAILURE);
    }

    // Записываем данные в канал
    if (fprintf(pipe, "%s", text) < 0) {
        perror("fprintf");
        exit(EXIT_FAILURE);
    }

    // Закрываем программный канал
    if (pclose(pipe) == -1) {
        perror("pclose");
        exit(EXIT_FAILURE);
    }

    return 0;
}
