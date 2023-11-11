#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 100

typedef struct line_info {
    unsigned int offset;
    unsigned int len;
} line_info_t;

unsigned int make_file_table(line_info_t file_table[], unsigned int table_size, unsigned int buffer_size, int fd) {
    char *buffer = (char *)malloc(sizeof(char) * buffer_size);
    unsigned int cur_buff_size = 0;
    unsigned int size = 0;
    unsigned int lines_count = 0;
    line_info_t info = {0, 0};

    while ((cur_buff_size = read(fd, buffer, sizeof(char) * buffer_size))) {
        for (unsigned int i = 0; i < cur_buff_size; ++i) {
            if (buffer[i] == '\n') {
                info.len = size + i - info.offset;
                file_table[lines_count++] = info;
                info.offset = size + i + 1;
            }
            if (lines_count >= table_size) {
                free(buffer);
                return lines_count;
            }
        }
        if (cur_buff_size < buffer_size && buffer[cur_buff_size - 1] != '\n') {
            info.len = size + cur_buff_size - info.offset;
            file_table[lines_count++] = info;
        }
        size += cur_buff_size;
    }

    free(buffer);
    return lines_count;
}

int print_line_by_number(int fd, line_info_t file_table[], unsigned int line_number) {
    line_info_t info = file_table[line_number - 1];
    if (lseek(fd, info.offset, SEEK_SET) == -1) {
        return -1;
    }
    char *buffer = (char *)malloc(sizeof(char) * info.len);
    read(fd, buffer, sizeof(char) * info.len);
    printf("%s\n", buffer);
    free(buffer);
    return info.len;
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

    line_info_t file_table[MAX_LINES];
    int lines_count = make_file_table(file_table, MAX_LINES, 1024, fd);

    unsigned int line_number = 0;
    scanf("%d", &line_number);

    while (line_number) {
        if (line_number > lines_count) {
            fprintf(stderr, "You are trying to get line %d, but the file contains only %d lines\n", line_number, lines_count);
            close(fd);
            return 3;
        }

        if (print_line_by_number(fd, file_table, line_number) == -1) {
            fprintf(stderr, "Can't move cursor in file\n");
            close(fd);
            return 4;
        }

        scanf("%d", &line_number);
    }

    close(fd);
    return 0;
}

