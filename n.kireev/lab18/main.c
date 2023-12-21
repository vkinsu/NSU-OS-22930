#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

int main(int argc, char* argv[])
{
    for (int i = 0; i < argc; i++)
    {
        if (argc > 1 && i == 0)
        {
            continue;
        }
        char path[260];

        if (argc == 1)
        {
            path[0] = '.';
        }
        else
        {
            for (int j = 0; j < sizeof(argv[i]) / sizeof(char); j++)
            {
                path[j] = argv[i][j];
                path[j + 1] = '\0';
            }
        }
        if (!access(path, F_OK))
        {
            struct stat stats;

            stat(path, &stats);

            if (stats.st_mode & S_IFREG)
            {
                printf("-");
            }
            else if (stats.st_mode & S_IFDIR)
            {
                printf("d");
            }
            else
            {
                printf("?");
            }

            if (stats.st_mode & S_IRUSR)
            {
                printf("r");
            }
            else
            {
                printf("-");
            }
            if (stats.st_mode & S_IWUSR)
            {
                printf("w");
            }
            else
            {
                printf("-");
            }
            if (stats.st_mode & S_IXUSR)
            {
                printf("x");
            }
            else
            {
                printf("-");
            }

            if (stats.st_mode & S_IRGRP)
            {
                printf("r");
            }
            else
            {
                printf("-");
            }
            if (stats.st_mode & S_IWGRP)
            {
                printf("w");
            }
            else
            {
                printf("-");
            }
            if (stats.st_mode & S_IXGRP)
            {
                printf("x");
            }
            else
            {
                printf("-");
            }

            if (stats.st_mode & S_IROTH)
            {
                printf("r");
            }
            else
            {
                printf("-");
            }
            if (stats.st_mode & S_IWOTH)
            {
                printf("w");
            }
            else
            {
                printf("-");
            }
            if (stats.st_mode & S_IXOTH)
            {
                printf("x");
            }
            else
            {
                printf("-");
            }

            printf(" % *lu", 3, (long unsigned int)stats.st_nlink);

            struct passwd* passwda = getpwuid(stats.st_uid);
            struct group* groupa = getgrgid(stats.st_gid);

            printf(" %s", passwda->pw_name);
            printf(" %s", groupa->gr_name);
            printf(" % *ld", 7, stats.st_size);

            char date[20];
            struct tm time;
            localtime_r(&stats.st_mtime, &time);
            strftime(date, sizeof(date), "%b %d  %Y", &time);
            printf(" %s %s\n", date, path);
        }
        else
        {
            printf("%s: No such file in directory\n", argv[i]);
        }
    }
}