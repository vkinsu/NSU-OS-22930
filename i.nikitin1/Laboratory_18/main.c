#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

void print_file_info(const char *filename) {
    struct stat file_stat;
    int err = 0;
    err = stat(filename, &file_stat);
    
    if (err == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    // Биты состояния файла
    switch (S_ISDIR(file_stat.st_mode))
    {
        case 1:
            printf("d ");
            break;
        default:
            switch (S_ISREG(file_stat.st_mode))
            {
                case 1:
                    printf("-");
                    break;
                default:
                    printf("?");
                    break;
            }
            break;
    }

    // Права доступа
    printf("%c%c%c%c%c%c%c%c%c ",
           (file_stat.st_mode & S_IRUSR) ? 'r' : '-',
           (file_stat.st_mode & S_IWUSR) ? 'w' : '-',
           (file_stat.st_mode & S_IXUSR) ? 'x' : '-',
           (file_stat.st_mode & S_IRGRP) ? 'r' : '-',
           (file_stat.st_mode & S_IWGRP) ? 'w' : '-',
           (file_stat.st_mode & S_IXGRP) ? 'x' : '-',
           (file_stat.st_mode & S_IROTH) ? 'r' : '-',
           (file_stat.st_mode & S_IWOTH) ? 'w' : '-',
           (file_stat.st_mode & S_IXOTH) ? 'x' : '-');

    // Количество связей(жестких ссылок)
    printf("%2ld ", (long)file_stat.st_nlink);

    // Имя собственника
    struct passwd *pwd = getpwuid(file_stat.st_uid);
    printf("%-8s ", pwd ? pwd->pw_name : "unknown");

    // Имя группы
    struct group *grp = getgrgid(file_stat.st_gid);
    printf("%-8s ", grp ? grp->gr_name : "unknown");

    // Размер файла (если это обычный файл)
    if (S_ISREG(file_stat.st_mode)) {
        printf("%8lld ", (long long)file_stat.st_size);
    } else {
        printf("          ");
    }

    // Дата модификации файла
    struct tm *tm_info = localtime(&file_stat.st_mtime);
    char date_string[13];
    strftime(date_string, sizeof(date_string), "%b %e %H:%M", tm_info);
    printf("%s ", date_string);

    // Имя файла
    printf("%s\n", filename);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Отсутствуют аргументы для передачи процесса!\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < argc; i++) {
        print_file_info(argv[i]);
    }

    return 0;
}