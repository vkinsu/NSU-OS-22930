#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <ucred.h>
#include <assert.h>
#include <ulimit.h>
#include <sys/resource.h>

int main(int argc, char *argv[]) {
    int c;
    extern char *optarg;
    extern int optind, optopt;

    extern char **environ;

    pid_t pid, pgid, ppid;
    uid_t euid, ruid;
    gid_t egid, rgid;
    long ulim;
    char *cwd;
    struct rlimit core_lim;

    while ((c = getopt(argc, argv, "U:uispdvV:cC:")) != -1) {
        switch (c) {
            case 'i':
                euid = geteuid();
                ruid = getuid();
                egid = getegid();
                rgid = getgid();
                printf(
                    "Effective user id = %d\nReal user id = %d\nEffective group id = %d\nReal group id = %d\n",
                    euid, ruid, egid, rgid
                );
                break;
            case 's':
                pid = getpid();
                if (setpgid(pid, pid) == -1) {
                    perror("Failed to set group id");
                }
                break;
            case 'p':
                pid = getpid();
                pgid = getpgrp();
                ppid = getppid();
                printf(
                    "Process id = %d\nParent process id = %d\nProcess group id = %d\n",
                    pid,
                    ppid,
                    pgid
                );
                break;
            case 'u':
                ulim = ulimit(UL_GETFSIZE); 
                if (ulim == -1) {
                    perror("Failed to get ulimit"); 
                    continue;
                }
                printf("ulimit = %ld\n", ulim * 512);
                break;
            case 'U':
                ulim = atol(optarg); 
                if (ulim % 512 != 0) {
                    fprintf(stderr, "New ulimit value is not divisible by 512\n");
                    continue;
                }
                ulim = ulimit(UL_SETFSIZE, ulim / 512);
                if (ulim == -1) {
                    perror("Failed to set ulimit"); 
                    continue;
                }
                break;
            case 'd':
                cwd = getcwd(NULL, 256);
                if (cwd == NULL) {
                    perror("Failed to get cwd");
                    continue;
                }
                printf("Current working directory: %s\n", cwd);
                free(cwd);
                cwd = NULL;
                break;
            case 'v':
                for (int i = 0; environ[i]; i++) {
                    printf("%s\n", environ[i]);
                }
                break;
            case 'V':
                if (putenv(optarg) != 0) {
                    perror("Failed to put env variable");
                }
                break;
            case 'c':
                if (getrlimit(RLIMIT_CORE, &core_lim) != 0)
                    perror("Failed to get core limit");
                printf("Hard core limit = %lu\nSoft core limit = %lu\n", core_lim.rlim_max, core_lim.rlim_cur);
                break;
            case 'C':
                if (getrlimit(RLIMIT_CORE, &core_lim) != 0)
                    perror("Failed to get core limit");
                core_lim.rlim_cur = atol(optarg); 
                if (setrlimit(RLIMIT_CORE, &core_lim) != 0)
                    perror("Failed to set core limit");
                break;
            case ':':
                fprintf(stderr, "Option -%c requires an operand\n", optopt);
                break;
            case '?':
                fprintf(stderr, "Unrecognized option: -%c\n", optopt);
                break;
        }
    }

    exit(0);
}