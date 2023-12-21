#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_LINE_LENGTH 256

struct LineTableEntry {
    off_t offset;
    size_t length;
};

int main() {
    char filename[100];
    int fd;

    printf("Enter the filename: ");
    scanf("%s", filename);

    if ((fd = open(filename, O_RDONLY)) == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    struct LineTableEntry *lineTable = NULL;
    off_t offset = 0;
    size_t lineNumber = 0;
    char buffer[MAX_LINE_LENGTH];

    while (read(fd, buffer, 1) > 0) {
        if (buffer[0] == '\n') {
            lineTable = realloc(lineTable, (lineNumber + 1) * sizeof(struct LineTableEntry));
            if (lineTable == NULL) {
                perror("Error allocating memory");
                exit(EXIT_FAILURE);
            }

            lineTable[lineNumber].offset = offset;
            lineTable[lineNumber].length = MAX_LINE_LENGTH;
            lineNumber++;
        } else {
            offset++;
        }
    }

    while (1) {
        size_t lineNum;
        printf("Enter the line number (0 to exit): ");
        scanf("%lu", &lineNum);

        if (lineNum == 0) {
            break;
        }

        if (lineNum <= lineNumber) {
            lseek(fd, lineTable[lineNum - 1].offset, SEEK_SET);
            read(fd, buffer, lineTable[lineNum - 1].length);
            printf("Line %lu: %s", lineNum, buffer);
        } else {
            printf("Line number out of range.\n");
        }
    }

    free(lineTable);
    close(fd);

    return 0;
}
