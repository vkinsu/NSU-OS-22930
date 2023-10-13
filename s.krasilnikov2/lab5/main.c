#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_LINE_LENGTH 100

void print_line(int fd, int line_num, const off_t* offsets, const int* lengths, int num_lines) {
    if (line_num < 1) {
        printf("Invalid line number\n");
        return;
    }

    if (line_num-1 > num_lines) {
        printf("Line does not exist\n");
        return;
    }

    off_t offset = offsets[line_num-1];
    int length = lengths[line_num-1];

    lseek(fd, offset, SEEK_SET);

    char buffer[MAX_LINE_LENGTH];
    read(fd, buffer, length);

    buffer[length] = '\0';

    printf("Line %d: %s\n", line_num, buffer);
}

int main() {
    char filename[100];
    printf("Enter filename: ");
    scanf("%s", filename);

    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        printf("File open failed");
        exit(1);
    }

    char ch;
    int num_lines = 0;

    // Count number of lines and store the offsets
    off_t offsets[MAX_LINE_LENGTH];
    int lengths[MAX_LINE_LENGTH];

    offsets[num_lines] = 0;
    lengths[num_lines] = 0;

    while (read(fd, &ch, 1) > 0) {
        if (ch == '\n') {
            num_lines++;
            offsets[num_lines] = lseek(fd, 0L, 1);
            lengths[num_lines] = 0;
        }
        else {
        lengths[num_lines]++;
        }
    }


    // Print table of line offsets and lengths
    printf("Line\tOffset\tLength\n");
    for (int i = 0; i <= num_lines; i++) {
    printf("%d\t\t%ld\t\t%d\n", i+1, offsets[i], lengths[i]);
    }

    int line_num;
    while (1) {
        printf("Enter line number (0 to exit): ");
        scanf("%d", &line_num);

        if (line_num == 0) {
            break;
        }

        print_line(fd, line_num, offsets, lengths, num_lines);
    }

    close(fd);

    return 0;
}
