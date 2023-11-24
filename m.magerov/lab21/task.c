#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

volatile sig_atomic_t beep_counter = 0;

void beep_handler(int) {
    write(STDOUT_FILENO, "\a", 1); // Печатаем символ звонка безопасно
    beep_counter++;
}

void quit_handler(int) {
    char buffer[50];
    int length = snprintf(buffer, sizeof(buffer), "Beep count: %d\n", beep_counter);
    write(STDOUT_FILENO, buffer, length);
    exit(0);
}

int main() {
    struct sigaction sa;

    // Установка обработчика для SIGINT
    sa.sa_handler = beep_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0; 
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("Error setting signal handler for SIGINT");
        exit(EXIT_FAILURE);
    }

    // Установка обработчика для SIGQUIT
    sa.sa_handler = quit_handler;
    if (sigaction(SIGQUIT, &sa, NULL) == -1) {
        perror("Error setting signal handler for SIGQUIT");
        exit(EXIT_FAILURE);
    }

    // Ожидаем сигналы бесконечно в основном потоке
    while (1) {
        pause();  // Останавливаем выполнение до получения сигнала
    }

    return 0;
}
