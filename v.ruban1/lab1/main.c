#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>

int main(int argc, char *argv[]) {
    int opt;
    int s_flag = 0, p_flag = 0, i_flag = 0, u_flag = 0, c_flag = 0, d_flag = 0, v_flag = 0, V_flag = 0;
    char *new_env_var = NULL;
    long new_ulimit = 0;
    long new_core_size = 0;

    while ((opt = getopt(argc, argv, "ispucdvV:U:C:")) != -1) {
        switch (opt) {
            case 'i':
                i_flag = 1;
                break;
            case 's':
                s_flag = 1;
                if (setpgrp() == -1) {
                    perror("setpgrp");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'p':
                p_flag = 1;
                printf("PID: %d\nPPID: %d\nPGID: %d\n", getpid(), getppid(), getpgrp());
                break;
            case 'u':
                u_flag = 1;
                {
                    struct rlimit limit;
                    if (getrlimit(RLIMIT_FSIZE, &limit) == 0) {
                        printf("ulimit: %lu\n", (unsigned long)limit.rlim_cur);
                    } else {
                        perror("getrlimit");
                        exit(EXIT_FAILURE);
                    }
                }
                break;
            case 'c':
                c_flag = 1;
                {
                    struct rlimit limit;
                    if (getrlimit(RLIMIT_CORE, &limit) == 0) {
                        printf("Core file size limit: %lu bytes\n", (unsigned long)limit.rlim_cur);
                    } else {
                        perror("getrlimit");
                        exit(EXIT_FAILURE);
                    }
                }
                break;
            case 'd':
                d_flag = 1;
                {
                    char cwd[4096];
                    if (getcwd(cwd, sizeof(cwd)) != NULL) {
                        printf("Current working directory: %s\n", cwd);
                    } else {
                        perror("getcwd");
                        exit(EXIT_FAILURE);
                    }
                }
                break;
            case 'v':
                v_flag = 1;
                {
                    extern char **environ;
                    for (char **env = environ; *env != NULL; env++) {
                        printf("%s\n", *env);
                    }
                }
                break;
            case 'V':
                V_flag = 1;
                new_env_var = optarg;
                putenv(optarg);
                printf("Setting environment variable: %s\n", new_env_var);
                break;
            case 'U':
                u_flag = 1;
                new_ulimit = atol(optarg);
                {
                    struct rlimit limit;
                    limit.rlim_cur = new_ulimit;
                    limit.rlim_max = new_ulimit;
                    if (setrlimit(RLIMIT_FSIZE, &limit) != 0) {
                        perror("setrlimit");
                        exit(EXIT_FAILURE);
                    }
                    printf("Setting ulimit to: %ld\n", new_ulimit);
                }
                break;
            case 'C':
                c_flag = 1;
                new_core_size = atol(optarg);
                {
                    struct rlimit limit;
                    limit.rlim_cur = new_core_size;
                    limit.rlim_max = new_core_size;
                    if (setrlimit(RLIMIT_CORE, &limit) != 0) {
                        perror("setrlimit");
                        exit(EXIT_FAILURE);
                    }
                    printf("Setting core file size limit to: %ld bytes\n", new_core_size);
                }
                break;
            default:
                fprintf(stderr, "Usage: %s [-ispucdvV:U:C:]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (i_flag) {
        printf("Real UID: %d\nEffective UID: %d\nReal GID: %d\nEffective GID: %d\n",
               getuid(), geteuid(), getgid(), getegid());
    }

    return 0;
}
