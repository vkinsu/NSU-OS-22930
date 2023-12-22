#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int count = 0;

void sigint() {
    printf("\a");
    count++;
}

void sigquit() {
    printf("SIGQUIT beeped %d times\n", count);
    exit(0);
}

int main() {
    signal(SIGINT, &sigint);
    signal(SIGQUIT, &sigquit);

    while (1);

    return 0;
}