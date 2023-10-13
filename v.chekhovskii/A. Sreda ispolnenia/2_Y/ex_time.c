#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

extern char *tzname[];

main() {
    time_t now = time(NULL);
    struct tm *sp = localtime(&now);

    printf("Local time (%d): ", atoi(tzname[0]));
    printf("%s", ctime(&now) );

    // California timezone is -7/-8 UTC, depending on DST
    now -= atoi(tzname[0])*3600;
    now -= 7*3600;
    now -= (sp->tm_isdst)*3600;

    printf("Time in California (%d): ", (-7 - sp->tm_isdst));
    printf("%s", ctime(&now) );
    return 0;
}
