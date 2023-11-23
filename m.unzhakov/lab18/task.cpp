#include <sys/types.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <grp.h>
#include <pwd.h>
#include <ctime>
#include <string.h>


int main(int argc, char* argv[]) {
    char* filepath;
    if (argc <= 1) {
        filepath = (char*)malloc(2);
        filepath[0] = '.';
        filepath[1] = '\0';
    }
    else {
        filepath = (char*)malloc(strlen(argv[1]) + 1);
        memcpy(filepath, argv[1], strlen(argv[1]) + 1);
    }
    struct stat file_info;
    
    stat(filepath, &file_info);

    if (file_info.st_mode & S_IFREG) {
        printf("-");
    }
    else if (file_info.st_mode & S_IFDIR) {
        printf("d");
    }
    else {
        printf("?");
    }
    if (file_info.st_mode & S_IRUSR) {
        printf("r");
    }
    else {
        printf("-");
    }
    if (file_info.st_mode & S_IWUSR) {
        printf("w");
    }
    else {
        printf("-");
    }
    if (file_info.st_mode & S_IXUSR) {
        printf("x");
    }
    else {
        printf("-");
    }
    if (file_info.st_mode & S_IRGRP) {
        printf("r");
    }
    else {
        printf("-");
    }
    if (file_info.st_mode & S_IWGRP) {
        printf("w");
    }
    else {
        printf("-");
    }
    if (file_info.st_mode & S_IXGRP) {
        printf("x");
    }
    else {
        printf("-");
    }
    if (file_info.st_mode & S_IROTH) {
        printf("r");
    }
    else {
        printf("-");
    }
    if (file_info.st_mode & S_IWOTH) {
        printf("w");
    }
    else {
        printf("-");
    }
    if (file_info.st_mode & S_IXOTH) {
        printf("x");
    }
    else {
        printf("-");
    }
    printf(" %lu ", (long unsigned int)file_info.st_nlink);
    struct group* user_group_info = getgrgid(file_info.st_gid);
    struct passwd* user_info = getpwuid(file_info.st_uid);
    printf("%s %s ", user_info->pw_name, user_group_info->gr_name);
    printf("%ld ", file_info.st_size);
    char date[50];
    struct tm t;
    localtime_r(&file_info.st_mtime, &t);
    strftime(date, sizeof(date), "%b %d %H:%M", &t);
    printf("%s %s\n", date, filepath);
    return 0;
}