#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_LINES 100

char *GLOBAL_FILE = NULL;
size_t FILE_SIZE = 0;

typedef struct line_info {
    unsigned int offset;
    unsigned int len;
} line_info_t;

unsigned int make_file_table(line_info_t file_table[], unsigned int table_size, char *file, size_t file_size) {
    unsigned int lines_count = 0;
    line_info_t info = {0, 0};
    
    for (size_t i = 0; i < file_size; ++i) {
        if (file[i] == '\n') {
            info.len = i - info.offset;
            file_table[lines_count++] = info;
            info.offset = i + 1;
        }
        
        if (lines_count >= table_size) {
            return lines_count;
        }
    }
    
    if (file[file_size - 1] != '\n') {
        info.len = file_size - info.offset;
        file_table[lines_count++] = info;
    }
    
    return lines_count;
}

int print_line_by_number(line_info_t file_table[], unsigned int line_number, char *file) {
    line_info_t info = file_table[line_number - 1];
    
    for (size_t i = 0; i < info.len; ++i) {
        printf("%c", file[i + info.offset]);
    }
    
    printf("\n");
    return info.len;
}

void print_all_file(int signo) {
    for (size_t i = 0; i < FILE_SIZE; ++i) {
        printf("%c", GLOBAL_FILE[i]);
    }
    
    printf("\n");
    exit(0);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Invalid amount of arguments\n");
        return 1;
    }
    
    int fd = open(argv[1], O_RDONLY);
    
    if (fd == -1) {
        fprintf(stderr, "Can't open file %s\n", argv[1]);
        return 2;
    }
    
    struct stat file_stat;
    
    if (fstat(fd, &file_stat) != 0) {
        fprintf(stderr, "Can't get file information\n");
        return 5;
    }
    
    char *file = NULL;
    
    if ((file = (char *) mmap(0, file_stat.st_size, PROT_READ, MAP_SHARED, fd, 0)) == MAP_FAILED) {
        fprintf(stderr, "Can't make mmap\n");
        return 7;
    }
    
    GLOBAL_FILE = file;
    FILE_SIZE = file_stat.st_size;
    line_info_t file_table[MAX_LINES];
    int lines_count = make_file_table(file_table, MAX_LINES, file, file_stat.st_size);
    unsigned int line_number = 1;
    signal(SIGALRM, print_all_file);
    
    while (line_number) {
        line_number = 0;
        alarm(5);
        
        if (!scanf("%d", &line_number)) {
            return 10;
        }
        
        alarm(0);
        
        if (line_number > lines_count) {
            fprintf(stderr, "You tried to get line %d, but the file contains only %d lines\n", line_number, lines_count);
            close(fd);
            return 3;
        }
        
        if (print_line_by_number(file_table, line_number, file) == -1) {
            fprintf(stderr, "Can't move cursor in file\n");
            return 4;
        }
    }
    
    close(fd);
    return 0;
}

