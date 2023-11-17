#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <ulimit.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <assert.h>


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

    while ((c = getopt(argc, argv, "ipsduU:vV:cC:")) != -1) {
        switch (c) {
            case 'i':
                pid = getpid();
                euid = geteuid();
                ruid = getuid();
                egid = getegid();
                rgid = getgid();
                printf(
                    "effective user id is: %d\nreal user id is: %d\neffective group id is: %d\nreal group id is: %d\n",
                    euid, ruid, egid, rgid
                );
                break;
            case 'p':
                pid = getpid();
                pgid = getpgrp();
                ppid = getppid();
                printf(
                    "process id is: %d\nparent process id is: %d\nprocess group id is: %d\n",
                    pid,
                    ppid,
                    pgid
                );
                break;
            case 's':
                pid = getpid();
                if (setpgid(pid, pid) == -1) {
                    perror("failed to set group");
                }
                break;
            case 'd':
                cwd = getcwd(NULL, 256);
                if (cwd == NULL) {
                    perror("failed to get cwd");
                    continue;
                }
                printf("current working directory is %s\n", cwd);
                free(cwd);
                cwd = NULL;
                break;
            case 'u':
                ulim = ulimit(UL_GETFSIZE); 
                if (ulim == -1) {
                    perror("failed to get ulimit"); 
                    continue;
                }
                printf("ulimit = %ld\n", ulim * 512);
                break;
            case 'U':
                ulim = atol(optarg); 
                if (ulim % 512 != 0) {
                    fprintf(stderr, "bad new ulimit value (is not divisible by 512)\n");
                    continue;
                }
                ulim = ulimit(UL_SETFSIZE, ulim / 512);
                if (ulim == -1) {
                    perror("failed to set ulimit"); 
                    continue;
                }
                break;
            case 'v':
                for (int i = 0; environ[i] != NULL; i++) {
                    printf("%s\n", environ[i]);
                }
                break;
            case 'V':
                if (putenv(optarg) != 0) {
                    perror("failed to put env variable");
                }
                break;
            case 'c':
                if (getrlimit(RLIMIT_CORE, &core_lim) != 0)
                    perror("failed to get core limit");
                printf("hard core limit is: %lu\nsoft core limit is: %lu\n", core_lim.rlim_max, core_lim.rlim_cur);
                break;
            case 'C':
                if (getrlimit(RLIMIT_CORE, &core_lim) != 0)
                    perror("failed to get core limit");
                core_lim.rlim_cur = atol(optarg); 
                if (setrlimit(RLIMIT_CORE, &core_lim) != 0)
                    perror("failed to set core limit");
                break;
            case ':':
                fprintf(stderr, "option -%c requires an operand\n", optopt);
                break;
            case '?':
                fprintf(stderr, "unrecognized option: -%c\n", optopt);
                break;
        }
    }
}
