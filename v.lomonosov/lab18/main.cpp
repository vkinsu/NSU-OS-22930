#include <sys/types.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>
#include <string.h>

int main(int argc, char* argv[]) {
    char* targetPath;

    if (argc <= 1) {
        targetPath = (char*)malloc(2);
        targetPath[0] = '.';
        targetPath[1] = '\0';
    } else {
        targetPath = (char*)malloc(strlen(argv[1]) + 1);
        strcpy(targetPath, argv[1]);
    }

    struct stat fileInfo;
    stat(targetPath, &fileInfo);

    if (S_ISREG(fileInfo.st_mode)) {
        printf("-");
    } else if (S_ISDIR(fileInfo.st_mode)) {
        printf("d");
    } else {
        printf("?");
    }

    printf((fileInfo.st_mode & S_IRUSR) ? "r" : "-");
    printf((fileInfo.st_mode & S_IWUSR) ? "w" : "-");
    printf((fileInfo.st_mode & S_IXUSR) ? "x" : "-");
    printf((fileInfo.st_mode & S_IRGRP) ? "r" : "-");
    printf((fileInfo.st_mode & S_IWGRP) ? "w" : "-");
    printf((fileInfo.st_mode & S_IXGRP) ? "x" : "-");
    printf((fileInfo.st_mode & S_IROTH) ? "r" : "-");
    printf((fileInfo.st_mode & S_IWOTH) ? "w" : "-");
    printf((fileInfo.st_mode & S_IXOTH) ? "x" : "-");

    printf(" %lu ", (long unsigned int)fileInfo.st_nlink);

    struct group* userGroupInfo = getgrgid(fileInfo.st_gid);
    struct passwd* userInfo = getpwuid(fileInfo.st_uid);

    printf("%s %s ", userInfo->pw_name, userGroupInfo->gr_name);
    printf("%ld ", fileInfo.st_size);

    char date[50];
    struct tm timeInfo;
    localtime_r(&fileInfo.st_mtime, &timeInfo);
    strftime(date, sizeof(date), "%b %d %H:%M", &timeInfo);
    printf("%s %s\n", date, targetPath);

    free(targetPath);

    return 0;
}
