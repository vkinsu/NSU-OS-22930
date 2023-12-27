#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

void print_permissions(mode_t mode) {
    printf((S_ISDIR(mode)) ? "d" : "-");
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
    struct stat file_info;
    struct passwd *pw;
    struct group *gr;
    
    for (int i = 1; i < argc; i++) {
        if (stat(argv[i], &file_info) == -1) {
            perror("Error file info");
            continue;
        }
        
        print_permissions(file_info.st_mode);
        
        printf(" %lu", (unsigned long)file_info.st_nlink);
        
        pw = getpwuid(file_info.st_uid);
        if (pw != NULL) {
            printf(" %s", pw->pw_name);
        } else {
            printf(" %d", file_info.st_uid);
        }
        
        gr = getgrgid(file_info.st_gid);
        if (gr != NULL) {
            printf(" %s", gr->gr_name);
        } else {
            printf(" %d", file_info.st_gid);
        }
        
        if (S_ISREG(file_info.st_mode)) {
            printf(" %ld", (long)file_info.st_size);
        } else {
            printf("      ");
        }
        
        char time_buf[20];
        strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", localtime(&file_info.st_mtime));
        printf(" %s", time_buf);
        
        printf(" %s\n", argv[i]);
    }
    
    return 0;
}