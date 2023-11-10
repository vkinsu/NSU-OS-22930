#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

#define MAX_LINE_LENGTH 100

void alarm_handler(int signum) {
    printf("\nTime is up! Printing the entire file...\n");
    char filename[] = "./lines.txt";
    int fd;
    if ((fd = open(filename, O_RDONLY)) == -1) {
        perror("open error");
        exit(1);
    }
    char buffer[MAX_LINE_LENGTH];
    ssize_t bytes;
    while ((bytes = read(fd, buffer, MAX_LINE_LENGTH)) > 0) {
        write(STDOUT_FILENO, buffer, bytes);
    }
    printf("\n");
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
        lseek(fd, offset_table[line_n - 1], SEEK_SET);
        read(fd, buffer, line_length[line_n]);
        buffer[line_length[line_n]] = '\0';
        printf("Line: %s\n", buffer);
    }

    close(fd);
    return 0;
}