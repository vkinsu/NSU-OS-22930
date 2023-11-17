#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <ucred.h>
#include <ulimit.h>
#include <assert.h>
#include <sys/resource.h>

int main(int argc, char *argv[])
{
    pid_t pid = getpid();
    ucred_t * cred = ucred_get(pid);
    if (!cred)
    {
        perror("Failed to obtain the user credential");
        return 1;
    }
    printf("Real user id = %d\nEffective user id = %d\n", ucred_getruid(cred), ucred_geteuid(cred));
    ucred_free(cred);

    FILE* fp = fopen("some.txt", "w+");
    if (!fp)
    {
        perror("File opening failed");
        return 2;
    }

    fclose(fp);

    if (setuid(getuid()) == -1)
    {
        perror("Failed equaling real and effective id");
        return 3;
    }

    pid = getpid();
    cred = ucred_get(pid);
    if (!cred)
    {
        perror("Failed to obtain the user credential");
        return 1;
    }
    printf("Real user id = %d\nEffective user id = %d\n", ucred_getruid(cred), ucred_geteuid(cred));
    ucred_free(cred);

    fp = fopen("some.txt", "w+");
    if (!fp)
    {
        perror("File opening failed");
        return 2;
    }
    fclose(fp);
    
    return 0;
}