#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

extern char *tzname[];


int main()
{
    time_t now;
    struct tm *sp, *gm;
    (void) time( &now );
    printf("Time of machine right now: %s", ctime( &now ) );
    gm = gmtime(&now);
    gm->tm_hour -= 7;
    time_t california_time = mktime(gm);
    printf("Time in California now: %s", ctime(&california_time));
    exit(0);
}