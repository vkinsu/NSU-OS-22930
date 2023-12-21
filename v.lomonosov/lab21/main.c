#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

int globalBeepCounter = 0;

void beepHandler(int signal) {
    printf("\007");
    globalBeepCounter++;
}

void quitSignalHandler(int signal) {
    printf("Beep count: %d\n", globalBeepCounter);
    exit(0);
}

int main() {
    signal(SIGINT, beepHandler);
    signal(SIGQUIT, quitSignalHandler);

    char userInput;
    while (1)
        scanf("%c", &userInput);

    return 0;
}
