#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

unsigned long beeps = 0;
char beepFlag = 0;



void beep(int signal) {
    beepFlag = 1;
}

void quit(int signal) {
    printf("\n\nTerminal beeped %ld times.\n", beeps);
    exit(0);
}



int main() {
    signal(SIGINT, beep); // ctrl+c
    signal(SIGQUIT, quit); // ctrl+\

    char userInput;
    while (1) {
        if (beepFlag) {
            beeps++;
            beepFlag = 0;
            printf("\a");
            fflush(stdout);
        }
    }

    return 1;
}