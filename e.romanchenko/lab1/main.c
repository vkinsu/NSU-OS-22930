#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <ucred.h>
#include <assert.h>
#include <ulimit.h>
#include <sys/resource.h>

int main(int argc, char *argv[]) {
    int option;
    extern char *optarg;
    extern int optind, optopt;

    extern char **environ;

    pid_t pid, process_group_id, parent_process_id;
    ucred_t *user_credentials;
    uid_t effective_user_id, real_user_id;
    gid_t effective_group_id, real_group_id;
    long ulimit_value;
    char *current_working_directory;
    struct rlimit core_limit;

    while ((option = getopt(argc, argv, "U:uispdvV:cC:")) != -1) {
        switch (option) {
            case 'c':
                if (getrlimit(RLIMIT_CORE, &core_limit) != 0)
                    perror("Failed to retrieve core limit");
                printf("Hard core limit = %lu\nSoft core limit = %lu\n", core_limit.rlim_max, core_limit.rlim_cur);
                break;
            case 'C':
                if (getrlimit(RLIMIT_CORE, &core_limit) != 0)
                    perror("Failed to retrieve core limit");
                core_limit.rlim_cur = atol(optarg);
                if (setrlimit(RLIMIT_CORE, &core_limit) != 0)
                    perror("Failed to set core limit");
                break;
            case 'u':
                ulimit_value = ulimit(UL_GETFSIZE);
                if (ulimit_value == -1) {
                    perror("Failed to retrieve ulimit");
                    continue;
                }
                printf("ulimit = %ld\n", ulimit_value * 512);
                break;
            case 'U':
                ulimit_value = atol(optarg);
                if (ulimit_value % 512 != 0) {
                    fprintf(stderr, "New ulimit value is not divisible by 512\n");
                    continue;
                }
                ulimit_value = ulimit(UL_SETFSIZE, ulimit_value / 512);
                if (ulimit_value == -1) {
                    perror("Failed to set ulimit");
                    continue;
                }
                break;
            case 'i':
                pid = getpid();
                user_credentials = ucred_get(pid);
                if (user_credentials == NULL) {
                    perror("Failed to obtain user credentials");
                    continue;
                }
                effective_user_id = ucred_geteuid(user_credentials);
                real_user_id = ucred_getruid(user_credentials);
                effective_group_id = ucred_getegid(user_credentials);
                real_group_id = ucred_getrgid(user_credentials);
                printf("Effective user id = %d\nReal user id = %d\nEffective group id = %d\nReal group id = %d\n",
                       effective_user_id, real_user_id, effective_group_id, real_group_id);
                ucred_free(user_credentials);
                break;
            case 's':
                pid = getpid();
                if (setpgid(pid, pid) == -1) {
                    perror("Failed to set process group id");
                }
                break;
            case 'p':
                pid = getpid();
                process_group_id = getpgrp();
                parent_process_id = getppid();
                printf("Process id = %d\nParent process id = %d\nProcess group id = %d\n", pid, parent_process_id,
                       process_group_id);
                break;
            case 'd':
                current_working_directory = getcwd(NULL, 256);
                if (current_working_directory == NULL) {
                    perror("Failed to retrieve current working directory");
                    continue;
                }
                printf("Current working directory: %s\n", current_working_directory);
                free(current_working_directory);
                current_working_directory = NULL;
                break;
            case 'v':
                for (int i = 0; environ[i]; i++) {
                    printf("%s\n", environ[i]);
                }
                break;
            case 'V':
                if (putenv(optarg) != 0) {
                    perror("Failed to set environment variable");
                }
                break;
            case ':':
                fprintf(stderr, "Option -%c requires an operand\n", optopt);
                break;
            case '?':
                fprintf(stderr, "Unrecognized option: -%c\n", optopt);
                break;
        }
    }
}
