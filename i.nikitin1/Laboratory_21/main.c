#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

volatile int count = 0;

void sigint_handler(int signo) {
    if (signo == SIGINT) {
        printf("Received SIGINT. Beep!\n");
        putchar('\a');  // Издать звуковой сигнал (может не работать на всех терминалах)
        count++;
    }
}

void sigquit_handler(int signo) {
    if (signo == SIGQUIT) {
        printf("\nReceived SIGQUIT. Total signals: %d\n", count);
        exit(EXIT_SUCCESS);
    }
}

int main() {
    struct sigaction sa_int, sa_quit;

    sa_int.sa_handler = sigint_handler;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;
	
	// SIGINT соответствует обработчику сигнала (прерывателю) под Ctrl+C
    if (sigaction(SIGINT, &sa_int, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    sa_quit.sa_handler = sigquit_handler;
    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags = 0;

	// SIGQUIT соответствует обработчику сигнала (прерывателю) под Ctrl+\\.
    if (sigaction(SIGQUIT, &sa_quit, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    printf("Press Ctrl+C to generate a beep. Press Ctrl+\\ to display the count and exit.\n");

    while (1) {
        // Бесконечный цикл
        sleep(1);  // Просто чтобы программа не использовала 100% процессорного времени
    }

    return 0;
}
