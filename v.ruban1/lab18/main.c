#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

void print_permissions(mode_t mode) {
    printf((S_ISDIR(mode)) ? "d" : (S_ISREG(mode)) ? "-" : "?");
    printf((mode & S_IRUSR) ? "r" : "-");
    printf((mode & S_IWUSR) ? "w" : "-");
    printf((mode & S_IXUSR) ? "x" : "-");
    printf((mode & S_IRGRP) ? "r" : "-");
    printf((mode & S_IWGRP) ? "w" : "-");
    printf((mode & S_IXGRP) ? "x" : "-");
    printf((mode & S_IROTH) ? "r" : "-");
    printf((mode & S_IWOTH) ? "w" : "-");
    printf((mode & S_IXOTH) ? "x" : "-");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file1> [file2] [file3] ...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct stat file_stat;
    struct passwd *pwd;
    struct group *grp;

    printf("%-2s %-10s %-10s %-8s %-20s %s\n", "Type", "Permissions", "Links", "Owner", "Modification Time", "File Name");

    for (int i = 1; i < argc; i++) {
        if (stat(argv[i], &file_stat) == -1) {
            perror("Error getting file information");
            continue;
        }

        printf("%-2s ", (S_ISDIR(file_stat.st_mode)) ? "d" : (S_ISREG(file_stat.st_mode)) ? "-" : "?");

        print_permissions(file_stat.st_mode);

        printf("%-2lu ", (unsigned long)file_stat.st_nlink);

        pwd = getpwuid(file_stat.st_uid);
        printf("%-10s ", (pwd != NULL) ? pwd->pw_name : "");

        grp = getgrgid(file_stat.st_gid);
        printf("%-10s ", (grp != NULL) ? grp->gr_name : "");

        printf("%-8ld ", (long)file_stat.st_size);

        char mtime_str[20];
        strftime(mtime_str, sizeof(mtime_str), "%b %d %H:%M", localtime(&file_stat.st_mtime));
        printf("%-20s ", mtime_str);

        printf("%s\n", argv[i]);
    }

    return 0;
}
