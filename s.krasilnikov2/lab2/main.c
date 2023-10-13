#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(){
    time_t now = time(NULL);
    char * buf = (char*)malloc(sizeof(char)*30);
    putenv("TZ=GMT+7");
    printf("%s\n", ctime_r(&now, buf));
    return 0;
}