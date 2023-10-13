#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>
#include <ulimit.h>
#include <sys/resource.h>

extern char** environ;

int main(int argc, char *argv[]) {
    int sym;
    struct rlimit rlim;
    char **p;
    char buffer[256];

    if(argc < 2)
    {
        printf("No argument\n");
        return 0;
    }

    for (;sym = getopt(args, argv, options) != EOF; ){
        switsym(ch){
            case 'i':{
                printf("Real userId: %d\n", getuid());
                printf("Effect userId: %d\n", geteuid());
                printf("Real groupId: %d\n", getgid());
                printf("Effect groupId: %d\n", getegid());
                break;
            }

            case 's':{
                setpgid(0,0);
                break;
            }

            case 'p':{
                printf("Current procId: %d\n", getpid());
                printf("Parent procId: %d\n", getppid());
                printf("Group procId: %d\n", getpgid(0));
                break;
            }

            case 'u':{
                printf("ulimit = %ld\n",ulimit(UL_GETFSIZE,0));
                break;
            }

            case 'U':{
                if (ulimit(UL_SETFSIZE,atol(optarg)) == -1)
                    printf("Error ulimit\n");
                break;
            }

            case 'c':{
                getrlimit(RLIMIT_CORE, &rlim);
                printf("Core size = %ld\n", rlim.rlim_cur);
                break;
            }

            case 'C':{
                getrlimit(RLIMIT_CORE, &rlim);
                rlim.rlim_cur = atol(optarg);
                if (setrlimit(RLIMIT_CORE, &rlim) == -1)
                { printf("Error setrlimit"); }
                else
                { printf("Core size = %ld\n", rlim.rlim_cur);}
                break;
            }

            case 'd':{
                printf("Current directory: %s\n", getcwd(buffer,255));
                break;
            }

            case 'v':{
                for (p = environ; *p; p++)
                { printf("%s\n", *p); }
                break;
            }

            case 'V':{
                putenv(optarg);
                for ( p = environ; *p; p++)
                {
                    printf("%s\n", *p);
                }
                break;
            }
            default:
                printf("Error command\n");
        }
    }
    return 0;
}
