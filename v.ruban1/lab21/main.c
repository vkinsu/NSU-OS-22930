#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

// Глобальная переменная для хранения количества сигналов SIGINT
int sigint_count = 0;

// Обработчик сигнала SIGINT
void sigint_handler(int signo) {
    if (signo == SIGINT) {
        sigint_count++;
        printf("\a"); // Издать звуковой сигнал на терминале
    }
}

// Обработчик сигнала SIGQUIT
void sigquit_handler(int signo) {
    if (signo == SIGQUIT) {
        printf("\nReceived SIGQUIT. Total SIGINT signals: %d\n", sigint_count);
        exit(EXIT_SUCCESS);
    }
}

int main() {
    // Установка обработчика сигнала SIGINT
    if (signal(SIGINT, sigint_handler) == SIG_ERR) {
        perror("Error setting up SIGINT handler");
        exit(EXIT_FAILURE);
    }

    // Установка обработчика сигнала SIGQUIT
    if (signal(SIGQUIT, sigquit_handler) == SIG_ERR) {
        perror("Error setting up SIGQUIT handler");
        exit(EXIT_FAILURE);
    }

    // Бесконечный цикл
    while (1) {
        sleep(1); // Ждать 1 секунду
    }

    return 0;
}
