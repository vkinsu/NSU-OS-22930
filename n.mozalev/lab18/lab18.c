#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

char get_type(struct stat *buf) {
    switch (buf->st_mode & S_IFMT) {
        case S_IFDIR:
            return 'd';
        case S_IFREG:
            return '-';
        default:
            return '?';
    }
}

void set_perm(int mode, char *perm) {
    if (mode & 1) {
        perm[2] = 'x';
    }
    if (mode & 2) {
        perm[1] = 'w';
    }
    if (mode & 4) {
        perm[0] = 'r';
    }
}

void get_permissions(mode_t mode, char *permissions) {
    mode_t permission_bits = mode & 0777;
    for (int i = 0; i < 9; ++i) {
        permissions[i] = '-';
    }
    set_perm(permission_bits, permissions + 6);
    set_perm(permission_bits >> 3, permissions + 3);
    set_perm(permission_bits >> 6, permissions);
}

void print_file_info(const char *fileName) {
    struct stat buf;
    if (stat(fileName, &buf) == -1) {
        perror("stat");
        exit(2);
    }
    char type = get_type(&buf);
    char permissions[10] = {0};
    get_permissions(buf.st_mode, permissions);
    nlink_t links = buf.st_nlink;
    char *mod_time = ctime(&buf.st_ctime);
    mod_time[strcspn(mod_time, "\n")] = '\0';
    off_t file_size = -1;
    char file_size_str[20] = {0};
    if (type == '-') {
        snprintf(file_size_str, 20, "%lld", buf.st_size);
    }
    struct passwd *pw = getpwuid(buf.st_uid);
    struct group *grp = getgrgid(buf.st_gid);
    size_t fileName_len = strlen(fileName);
    char *file_name_without_path = (char *)calloc(fileName_len + 1, sizeof(char));
    int last_slash_pos = -1;
    for (int i = fileName_len - 1; i >= 0; --i) {
        if (fileName[i] == '/') {
            last_slash_pos = i;
            break;
        }
    }
    strncpy(file_name_without_path, fileName + last_slash_pos + 1, fileName_len - last_slash_pos - 1);
    printf("%c%-10s\t%-10d\t%-10s\t%-10s\t%-10s\t%-10s\t%-10s\n", type, permissions, links, file_size_str, pw->pw_name, grp->gr_name, mod_time, file_name_without_path);
    free(file_name_without_path);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Wrong arguments number");
        return 1;
    }
    for (int i = 1; i < argc; ++i) {
        print_file_info(argv[i]);
    }
    return 0;
}