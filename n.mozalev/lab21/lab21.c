#include <stdio.h>
#include <signal.h> 
int count = 0;

void signalHandler(int signal) {
    if (signal == SIGINT) {
        count++; 
        printf("\aBeep!\n");
    } else if (signal == SIGQUIT) {
        printf("Signal received %d times. Exiting...\n", count);
        exit(0);
    }
}

int main() {
    signal(SIGINT, signalHandler);
    signal(SIGQUIT, signalHandler);

    while (1) {
        // Infinite loop
    }

    return 0;
}

