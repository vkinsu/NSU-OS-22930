#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <ulimit.h>
#include <sys/resource.h>

pid_t processId, processGroupId, parentProcessId;
uid_t effectiveUserId, realUserId;
gid_t effectiveGroupId, realGroupId;
long userLimit;
char *currentWorkingDirectory;
struct rlimit coreLimit;

void displayUserIdentity() {
    processId = getpid();
    effectiveUserId = geteuid();
    realUserId = getuid();
    effectiveGroupId = getegid();
    realGroupId = getgid();
    printf("Effective user id is: %d\nReal user id is: %d\nEffective group id is: %d\nReal group id is: %d\n",
           effectiveUserId, realUserId, effectiveGroupId, realGroupId);
}

void displayProcessInfo() {
    processId = getpid();
    processGroupId = getpgrp();
    parentProcessId = getppid();
    printf("Process id is: %d\nParent process id is: %d\nProcess group id is: %d\n", processId, parentProcessId,
           processGroupId);
}

void setProcessGroup() {
    processId = getpid();
    if (setpgid(processId, processId) == -1) {
        perror("Failed to set process group");
    }
}

void displayCurrentWorkingDirectory() {
    currentWorkingDirectory = getcwd(NULL, 256);
    if (currentWorkingDirectory == NULL) {
        perror("Failed to get current working directory");
        return;
    }
    printf("Current working directory is %s\n", currentWorkingDirectory);
    free(currentWorkingDirectory);
    currentWorkingDirectory = NULL;
}

void displayUserLimit() {
    userLimit = ulimit(UL_GETFSIZE);
    if (userLimit == -1) {
        perror("Failed to get user limit");
        return;
    }
    printf("User limit = %ld\n", userLimit * 512);
}

void setUserLimit(char *limitValue) {
    userLimit = atol(limitValue);
    if (userLimit % 512 != 0) {
        fprintf(stderr, "Bad new user limit value (is not divisible by 512)\n");
        return;
    }
    userLimit = ulimit(UL_SETFSIZE, userLimit / 512);
    if (userLimit == -1) {
        perror("Failed to set user limit");
    }
}

void displayEnvironmentVariables() {
    for (int i = 0; environ[i] != NULL; i++) {
        printf("%s\n", environ[i]);
    }
}

void setEnvironmentVariable(char *variable) {
    if (putenv(variable) != 0) {
        perror("Failed to set environment variable");
    }
}

void displayCoreLimits() {
    if (getrlimit(RLIMIT_CORE, &coreLimit) != 0)
        perror("Failed to get core limits");
    printf("Hard core limit is: %lu\nSoft core limit is: %lu\n", coreLimit.rlim_max, coreLimit.rlim_cur);
}

void setCoreLimit(char *limitValue) {
    if (getrlimit(RLIMIT_CORE, &coreLimit) != 0)
        perror("Failed to get core limits");
    coreLimit.rlim_cur = atol(limitValue);
    if (setrlimit(RLIMIT_CORE, &coreLimit) != 0)
        perror("Failed to set core limits");
}

int main(int argc, char *argv[]) {
    int option;
    extern char *optarg;
    extern int optind, optopt;

    while ((option = getopt(argc, argv, "ipsduU:vV:cC:")) != -1) {
        switch (option) {
            case 'i':
                displayUserIdentity();
                break;
            case 'p':
                displayProcessInfo();
                break;
            case 's':
                setProcessGroup();
                break;
            case 'd':
                displayCurrentWorkingDirectory();
                break;
            case 'u':
                displayUserLimit();
                break;
            case 'U':
                setUserLimit(optarg);
                break;
            case 'v':
                displayEnvironmentVariables();
                break;
            case 'V':
                setEnvironmentVariable(optarg);
                break;
            case 'c':
                displayCoreLimits();
                break;
            case 'C':
                setCoreLimit(optarg);
                break;
            case ':':
                fprintf(stderr, "Option -%c requires an operand\n", optopt);
                break;
            case '?':
                fprintf(stderr, "Unrecognized option: -%c\n", optopt);
                break;
        }
    }
    return 0;
}
