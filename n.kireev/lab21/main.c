#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <signal.h>

int num = 0;

void intHandler(int dummy)
{
    printf("\a");
    num = num + 1;
}

void quitHandler(int dummy)
{
    printf("Signal beeped %d times. Endstation.Eden\n", num);
    exit(0);
}

int main(void)
{
    signal(SIGINT, intHandler);
    while (1)
    {
        signal(SIGQUIT, quitHandler);
        char c = getc(stdin);
    }
}
