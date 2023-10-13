#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <ulimit.h>
#include <sys/resource.h>

extern char **environ;

main(int argc, char *argv[]) {
    char options[] = "ispuU:cC:dvV:h";  // valid options
    int c = 0;
    //printf("argc equals %d\n", argc);

    while ((c = getopt(argc, argv, options)) != EOF) {
        switch (c) {
        case 'i':
            printf("Real user id: %d\n", getuid());
            printf("Effective user id: %d\n", geteuid());
            printf("Real group id: %d\n", getgid());
            printf("Effective group id: %d\n", getegid());
            break;

        case 's':
            setpgid(0, 0);
            printf("Process set as process group leader.\n");
            break;
        case 'p':
            //system("ps a");
            printf("Process id: %d\n", getpid());
            printf("Parent id: %d\n", getppid());
            printf("Group id: %d\n", getpgrp());
            break;

        case 'u':
            printf("Maximum allowed file size: ");
            int ulim = ulimit(UL_GETFSIZE, 0)*512;
            if (ulim == -512) {
                printf("Unlimited!\n");
            }
            else {
                printf("%d bytes\n", ulim);
            }
            break;
        case 'U':
            if (atoi(optarg) < -0) {
                printf("Failed to set new maximum allowed file size: invalid input value.\n");
                return 1;
            }
            else {
                ulimit(UL_SETFSIZE, atoi(optarg));

                ulim = ulimit(UL_GETFSIZE, 0)*512;
                printf("Set new maximum allowed file size to %d bytes.\n", ulim);
            }
            break;

        case 'c':
            printf(""); // a label can only be part of a statement and a declaration is not a statement
            struct rlimit rlim;
            getrlimit(RLIMIT_CORE, &rlim);
            printf("Core file size: %d bytes\n", (int)rlim.rlim_cur*512);
            break;
        case 'C':
            if (atoi(optarg) < 0) {
                printf("Failed to set new core file size: invalid input value.\n");
                return 1;
            }
            else {
                struct rlimit rlim;
                getrlimit(RLIMIT_CORE, &rlim);
                rlim.rlim_cur = atoi(optarg);
                setrlimit(RLIMIT_CORE, &rlim);

                getrlimit(RLIMIT_CORE, &rlim);
                printf("Set new core file size to %d bytes.\n", (int)rlim.rlim_cur*512);
            }
            break;

        case 'd':
            printf(""); // a label can only be part of a statement and a declaration is not a statement
            char path[4096];
            printf("Working directory: %s\n", getcwd(path, 4096));
            break;

        case 'v':
            printf("Environment variables:\n");
            int evar = 0;
            while(environ[evar]) {
                printf("%s\n", environ[evar++]);
            }
            break;
        case 'V':
            if (!strchr(optarg, '=')) {
                printf("Failed to add enviornment variable: invalid input value.\n");
                return 1;
            }
            else {
                printf("Adding enviornment variable: %s\n", optarg);
                putenv(optarg);
            }
            break;

        case 'h':
            printf("-i :: Prints real and effective user and group IDs.\n");
            printf("-s :: Process becomes the group leader.\n");
            printf("-p :: Prints the process, parent process, and process group identifiers.\n");
            printf("-u :: Prints the value of ulimit.\n");
            printf("-Unew_ulimit :: Changes the value of ulimit.\n");
            printf("-c :: Prints the size in bytes of the core file that can be created.\n");
            printf("-Csize :: Changes the size of the core file.\n");
            printf("-d :: Prints the current working directory.\n");
            printf("-v :: Prints environment variables and their values.\n");
            printf("-Vname=value :: Introduces a new variable into the environment or changes the value of an existing variable.\n");
            printf("-h :: Prints this help message.\n");
            break;
        case ':':
            printf("Option '%c' requires an argument.\n", optopt);
            break;
        default:
            printf("Option '%c' is invalid; use 'h' for help.\n", optopt);
        }
    }
}
