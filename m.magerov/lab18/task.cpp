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

// Функция для печати разрешений файла
void print_permissions(mode_t mode) {
    char permissions[10];
    snprintf(permissions, sizeof(permissions), "%c%c%c%c%c%c%c%c%c",
             (mode & S_IRUSR) ? 'r' : '-', (mode & S_IWUSR) ? 'w' : '-',
             (mode & S_IXUSR) ? 'x' : '-', (mode & S_IRGRP) ? 'r' : '-',
             (mode & S_IWGRP) ? 'w' : '-', (mode & S_IXGRP) ? 'x' : '-',
             (mode & S_IROTH) ? 'r' : '-', (mode & S_IWOTH) ? 'w' : '-',
             (mode & S_IXOTH) ? 'x' : '-');
    printf("%s", permissions);
}

// Функция для печати информации о файле
void print_file_info(const char *filepath) {
    struct stat file_info;
    if (stat(filepath, &file_info) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }
    printf((S_ISREG(file_info.st_mode)) ? "-" : (S_ISDIR(file_info.st_mode)) ? "d" : "?");
    print_permissions(file_info.st_mode);

    printf(" %lu ", (long unsigned int)file_info.st_nlink);
    struct group* user_group_info = getgrgid(file_info.st_gid);
    struct passwd* user_info = getpwuid(file_info.st_uid);
    if (user_info == NULL || user_group_info == NULL) {
        perror("getpwuid/getgrgid");
        exit(EXIT_FAILURE);
    }
    printf("%s %s ", user_info->pw_name, user_group_info->gr_name);

    printf("%ld ", file_info.st_size);

    char date[50];
    struct tm lt;
    localtime_r(&file_info.st_mtime, &lt);
    strftime(date, sizeof(date), "%b %d %H:%M", &lt);
    printf("%s %s\n", date, filepath);
}

int main(int argc, char* argv[]) {
    char* filepath;
    if (argc <= 1) {
        filepath = ".";
    } else {
        filepath = argv[1];
    }

    print_file_info(filepath);
    return 0;
}
