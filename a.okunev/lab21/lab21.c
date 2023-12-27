#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <signal.h>

int count = 0;

void sigint_handler(int dummy)
{
    printf("\a");
    count++;
}

void sigquit_handler(int dummy)
{
    printf("Signal beeped %d times.\n", count);
    exit(0);
}

int main(void)
{
    signal(SIGQUIT, sigquit_handler);
    while (1)
    {
        signal(SIGINT, sigint_handler);
        char c = getc(stdin);
    }
}