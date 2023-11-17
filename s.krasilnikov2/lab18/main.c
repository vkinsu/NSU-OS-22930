#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

void print_permissions(mode_t mode) {
    printf("%c", S_ISDIR(mode) ? 'd' : '-');
    printf("%c", (mode & S_IRUSR) ? 'r' : '-');
    printf("%c", (mode & S_IWUSR) ? 'w' : '-');
    printf("%c", (mode & S_IXUSR) ? 'x' : '-');
    printf("%c", (mode & S_IRGRP) ? 'r' : '-');
    printf("%c", (mode & S_IWGRP) ? 'w' : '-');
    printf("%c", (mode & S_IXGRP) ? 'x' : '-');
    printf("%c", (mode & S_IROTH) ? 'r' : '-');
    printf("%c", (mode & S_IWOTH) ? 'w' : '-');
    printf("%c", (mode & S_IXOTH) ? 'x' : '-');
}

void print_file_info(const char* filename) {
    struct stat file_stat;
    if (stat(filename, &file_stat) == -1) {
        perror("Error getting file information");
        return;
    }

    print_permissions(file_stat.st_mode);

    printf(" %2lu", file_stat.st_nlink);

    struct passwd* owner_info = getpwuid(file_stat.st_uid);
    struct group* group_info = getgrgid(file_stat.st_gid);

    printf(" %-10s %-10s", (owner_info != NULL) ? owner_info->pw_name : "unknown",
           (group_info != NULL) ? group_info->gr_name : "unknown");

    if (S_ISREG(file_stat.st_mode)) {
        printf(" %8lld", (long long)file_stat.st_size);
    } else {
        printf(" %-8s", "");
    }

    char time_buffer[20];
    strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", localtime(&file_stat.st_mtime));
    printf(" %s", time_buffer);

    printf(" %s\n", strrchr(filename, '/') ? strrchr(filename, '/') + 1 : filename);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; ++i) {
        print_file_info(argv[i]);
    }

    return 0;
}
