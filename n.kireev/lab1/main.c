#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <ucred.h>
#include <ulimit.h>
#include <assert.h>
#include <sys/resource.h>

int main(int argc, char *argv[]) {
    int c = 0;
    extern char *optarg, **environ;
    extern int optind, optopt;

    ucred_t *cred;
    pid_t pid;//, pgid, ppid;
    long ulim;
    char *cwd;
    struct rlimit core_lim;
    while (1) {
		c = getopt(argc, argv, "ispuU:cC:dvV:");
		if (c == -1)
		{
			break;
		}
        switch (c) {
            case 'i':
                pid = getpid();
                cred = ucred_get(pid);
                if (!cred) {
                    perror("Failed to obtain the user credential");
                    continue;
                }
                printf("Real user id = %d\nEffective user id = %d\nReal groud id = %d\nEffective group id = %d\n", ucred_getruid(cred), ucred_geteuid(cred), ucred_getrgid(cred), ucred_getegid(cred));
                ucred_free(cred);
                break;
            case 's':
                pid = getpid();
                if (setpgid(pid, pid) == -1)
				{
                    perror("Failed to set group id");
                }
                break;
            case 'p':
                printf("Process id = %d\nParent process id = %d\nProcess group id = %d\n", getpid(), getpgrp(), getppid());
                break;
            case 'u':
                ulim = ulimit(UL_GETFSIZE); 
                if (ulim == -1)
				{
                    perror("Failed to get ulimit"); 
                    continue;
                }
                printf("ulimit = %ld\n", ulim * 512);
                break;
            case 'U':
                ulim = atol(optarg); 
                if (ulim % 512 != 0)
				{
                    perror("Ulimit value need to be divisible by 512\n");
                    continue;
                }
                ulim = ulimit(UL_SETFSIZE, ulim / 512);
                if (ulim == -1)
				{
                    perror("Failed to set ulimit"); 
                    continue;
                }
                break;
			case 'c':
                if (getrlimit(RLIMIT_CORE, &core_lim) != 0)
				{
                    perror("Failed to get core limit");
				}
                printf("Hard core limit = %lu\nSoft core limit = %lu\n", core_lim.rlim_max, core_lim.rlim_cur);
                break;
            case 'C':
                if (getrlimit(RLIMIT_CORE, &core_lim) != 0)
				{
                    perror("Failed to get core limit");
				}
                core_lim.rlim_cur = atol(optarg); 
                if (setrlimit(RLIMIT_CORE, &core_lim) != 0)
				{
                    perror("Failed to set core limit");
				}
                break;
            case 'd':
                cwd = getcwd(NULL, 256);
                if (cwd == NULL)
				{
                    perror("Failed to get cwd");
                    continue;
                }
                printf("Current working directory: %s\n", cwd);
                free(cwd);
                cwd = NULL;
                break;
            case 'v':
                for (int i = 0; environ[i]; i++)
				{
                    printf("%s\n", environ[i]);
                }
                break;
            case 'V':
                if (putenv(optarg) != 0)
				{
                    perror("Failed to put environ variable");
                }
                break;
            case ':':
                fprintf(stderr, "Option -%c requires value\n", optopt);
                break;
            case '?':
                fprintf(stderr, "Unrecognized option: -%c\n", optopt);
                break;
        }
    }
}
