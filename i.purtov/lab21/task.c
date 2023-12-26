#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

int beep_counter = 0;

void beep(int) {
    printf("\007");
    beep_counter++;
}

void quit_sig(int) { 
    printf("Beep count: %d\n", beep_counter);
    exit(0);
}

int main() {
    signal(SIGINT, beep);
    signal(SIGQUIT, quit_sig);
    char x;
    while (1)
        scanf("%c", &x);
    return 0;
}