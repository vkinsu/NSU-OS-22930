#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        struct stat currentStat;
        if (stat(argv[i], &currentStat)) {
            printf("Unable to get stat for %s\n", argv[i]);
            continue;
        }

        printf("%s ", S_ISDIR(currentStat.st_mode) ? "d" : (S_ISREG(currentStat.st_mode) ? "-" : "?"));

        printf("%c", (currentStat.st_mode & S_IRUSR) ? 'r' : '-');
        printf("%c", (currentStat.st_mode & S_IWUSR) ? 'w' : '-');
        printf("%c", (currentStat.st_mode & S_IXUSR) ? 'x' : '-');
        printf("%c", (currentStat.st_mode & S_IRGRP) ? 'r' : '-');
        printf("%c", (currentStat.st_mode & S_IWGRP) ? 'w' : '-');
        printf("%c", (currentStat.st_mode & S_IXGRP) ? 'x' : '-');
        printf("%c", (currentStat.st_mode & S_IROTH) ? 'r' : '-');
        printf("%c", (currentStat.st_mode & S_IWOTH) ? 'w' : '-');
        printf("%c", (currentStat.st_mode & S_IXOTH) ? 'x' : '-');

        printf(" %ld\t", currentStat.st_nlink);

        printf("%s\t", getpwuid(currentStat.st_uid)->pw_name);
        printf("%s\t", getgrgid(currentStat.st_gid)->gr_name);
        
        if (S_ISREG(currentStat.st_mode)) {
            printf("%ld\t", (long)currentStat.st_size);
        } else {
            printf("   \t");
        }

        char currDate[64];
        strftime(currDate, sizeof(currDate), "%b %d %H:%M", localtime(&currentStat.st_mtime));
        printf("%s\t", currDate);

        printf("%s\n", argv[i]);
    }

    return 0;
}
