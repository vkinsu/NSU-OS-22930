#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <time.h>
#include <libgen.h>

struct stat st;

void set_file_status(char* file_status) {
    if (S_ISDIR(st.st_mode))
        *file_status = 'd';
    else if (S_ISREG(st.st_mode))
        *file_status = '-';
    else
        *file_status = '?';
}

void set_access_rights_for_the_owner(char* access_rights) {
    access_rights[0] = st.st_mode & S_IRUSR ? 'r' : '-';
    access_rights[1] = st.st_mode & S_IWUSR ? 'w' : '-';
    access_rights[2] = st.st_mode & S_IXUSR ? 'x' : '-';
}

void set_access_rights_for_the_group(char* access_rights) {
    access_rights[3] = st.st_mode & S_IRGRP ? 'r' : '-';
    access_rights[4] = st.st_mode & S_IWGRP ? 'w' : '-';
    access_rights[5] = st.st_mode & S_IXGRP ? 'x' : '-';
}

void set_access_rights_for_the_rest(char* access_rights) {
    access_rights[6] = st.st_mode & S_IROTH ? 'r' : '-';
    access_rights[7] = st.st_mode & S_IWOTH ? 'w' : '-';
    access_rights[8] = st.st_mode & S_IXOTH ? 'x' : '-';
    access_rights[9] = 0;
}

void set_access_rights(char* access_rights) {
    set_access_rights_for_the_owner(access_rights);
    set_access_rights_for_the_group(access_rights);
    set_access_rights_for_the_rest(access_rights);
}

void set_owner_name(char* owner_name) {
    struct passwd* p = getpwuid(getuid());
    strncpy(owner_name, p->pw_name, 20);
}

void set_group_name(char* group_name) {
    struct group* g = getgrgid(getgid());
    strncpy(group_name, g->gr_name, 20);
}

void set_file_size(char file_status, char* file_size) {
    if (file_status == '-')
        snprintf(file_size, 10, "%lld", st.st_size);
    else
        *file_size = 0;
}

void set_date(char* date) {
    struct tm* t = localtime(&st.st_mtime);
    strftime(date, 13, "%e %b %R", t);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("There is no any parameters.");
        return 1;
    }
    int i;
    for (i = 1; i < argc; ++i) {
        stat(argv[i], &st);

        char file_status;
        char access_rights[10];
        char owner_name[20];
        char group_name[20];
        char file_size[10];
        char date[13];
        char* file_name;

        set_file_status(&file_status);
        set_access_rights(access_rights);
        set_owner_name(owner_name);
        set_group_name(group_name);
        set_file_size(file_status, file_size);
        set_date(date);
        file_name = basename(argv[i]);

        printf("%c%s %-20s %-20s %-10s %-13s %-s\n", file_status, access_rights, owner_name, group_name, file_size, date, file_name);
    }
    return 0;
}