#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <ulimit.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <assert.h>


int main(int argv, char* argc[]) {
    if (argv == 1) {
        printf("you need to put your filepath into parameters of startup\n");
        return 1;
    }
    printf("real user id is: %d\neffective user id is: %d\n", getuid(), geteuid());

    setuid(geteuid());

    FILE* fin = fopen(argc[1], "r");

    if (fin != NULL) {
        fclose(fin);
        printf("file sucessfully opened\n");
    }
    else {
        perror("can't open file\n");
    }
    return 0;
}