#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>

int main()
{
    if (putenv("TZ=America/Tijuana") != 0)
    {
        perror("Failed to set env variable");
        return 1;
    }

    time_t t = time(0);
    char datetime[30] = {};

    if (!ctime_r(&t, datetime, 30))
    {
        perror("Failed to print current time");
	      return 2;
    }

    printf(datetime);
    return 0;
}
