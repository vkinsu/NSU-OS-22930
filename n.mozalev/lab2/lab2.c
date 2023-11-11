#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
int main() {
    time_t now;
    struct tm *sp;
    char buffer[80];

    putenv("TZ=PST+8:00:00PDT,M3.1.0,M10.1.0");
    time(&now);

    sp = localtime(&now);
    strftime(buffer, sizeof(buffer), "%m/%d/%Y %H:%M %Z", sp);
    printf("%s\n", buffer);

    return 0;
}
