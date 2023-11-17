#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define MAX_LINE_LENGTH 100

void alarm_handler(int signum) {
    printf("\nTime is up! Printing the entire file...\n");
    char filename[] = "./lines.txt";
    int fd;
    if ((fd = open(filename, O_RDONLY)) == -1) {
        perror("open error");
        exit(1);
    }
    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        perror("fstat error");
        exit(1);
    }
    char *file_in_memory = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (file_in_memory == MAP_FAILED) {
        perror("mmap error");
        exit(1);
    }
    write(STDOUT_FILENO, file_in_memory, sb.st_size);
    munmap(file_in_memory, sb.st_size);
    close(fd);
    exit(0);
}

int main() {
    signal(SIGALRM, alarm_handler);

    char filename[] = "./lines.txt";
    int fd;

    if ((fd = open(filename, O_RDONLY)) == -1) {
        perror("open error");
        exit(1);
    }

    int offset_table[100];
    int line_length[100];
    offset_table[0] = 0;
    line_length[0] = 0;
    int offset = 0;
    int line_num = 1;
    int line_n;
    char buffer[MAX_LINE_LENGTH];
    ssize_t bytes;

    while ((bytes = read(fd, buffer, MAX_LINE_LENGTH)) > 0) {
        for (int i = 0; i < bytes; i++) {

            offset += 1;
            if (line_num > 1 && buffer[i] == '\n') {
                offset_table[line_num] = offset;
                line_length[line_num] = offset_table[line_num] - offset_table[line_num - 1] - 1;
                line_num += 1;
            } else if (buffer[i] == '\n' && line_num == 1) {
                offset_table[1] = offset;
                line_length[1] = i;
                line_num += 1;
            }
        }
    }

    offset_table[line_num] = offset;
    line_length[line_num] = offset - offset_table[line_num - 1];

    printf("Table of offsets and line lengths:\n");
    for (int i = 1; i <= line_num; i++) {
        printf("Line %d: Offset %d, Length %d\n", i, offset_table[i], line_length[i]);
    }

    alarm(5); 

    printf("Enter the line number (0 to exit): ");
    scanf("%d", &line_n);
    if (line_n == 0) {
        close(fd);
        exit(0);
    } else if (line_n < 0 || line_n > line_num) {
        perror("Invalid line number\n");
        close(fd);
        exit(1);
    } else {
        struct stat sb;
        if (fstat(fd, &sb) == -1) {
            perror("fstat error");
            exit(1);
        }
        char *file_in_memory = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
        if (file_in_memory == MAP_FAILED) {
            perror("mmap error");
            exit(1);
        }
        printf("Line: %.*s\n", line_length[line_n], file_in_memory + offset_table[line_n - 1]);
        munmap(file_in_memory, sb.st_size);
    }

    close(fd);
    return 0;
}