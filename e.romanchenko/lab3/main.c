#include <stdio.h>
#include <stdlib.h>
#include <archive.h>

int main(){
    printf("real_id: %d\n", getuid());
    printf("effective_id: %d\n", geteuid());

    FILE* f = fopen("file.txt", "r");
    if (f == NULL)
        perror("Error");
    else
        fclose(f);

    putenv("TZ=PST8PDT");
    setuid(getuid());

    printf("real_id: %d\n", getuid());
    printf("effective_id: %d\n", geteuid());

    f = fopen("file.txt", "r");
    if (f == NULL)
        perror("Error: ");
    else
        fclose(f);
    return 0;
}