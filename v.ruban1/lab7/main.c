#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>

#define MAX_LINE_LENGTH 256
#define TIMEOUT 5

struct LineTableEntry {
    off_t offset;
    size_t length;
};

volatile sig_atomic_t timeout_flag = 0;

void handle_timeout(int signum) {
    timeout_flag = 1;
}

int main() {
    char filename[100];
    int fd;

    signal(SIGALRM, handle_timeout);

    printf("Enter the filename: ");
    scanf("%s", filename);

    if ((fd = open(filename, O_RDONLY)) == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    off_t fileSize = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    char *fileContent = mmap(NULL, fileSize, PROT_READ, MAP_PRIVATE, fd, 0);
    if (fileContent == MAP_FAILED) {
        perror("Error mapping file to memory");
        exit(EXIT_FAILURE);
    }

    struct LineTableEntry *lineTable = NULL;
    size_t lineNumber = 0;
    off_t offset = 0;

    alarm(TIMEOUT);

    for (off_t i = 0; i < fileSize; i++) {
        if (fileContent[i] == '\n') {
            lineTable = realloc(lineTable, (lineNumber + 1) * sizeof(struct LineTableEntry));
            if (lineTable == NULL) {
                perror("Error allocating memory");
                munmap(fileContent, fileSize);
                close(fd);
                exit(EXIT_FAILURE);
            }

            lineTable[lineNumber].offset = offset;
            lineTable[lineNumber].length = i - offset;
            lineNumber++;

            offset = i + 1;  // Move offset to the next character after '\n'
        }
    }

    alarm(0);

    if (!timeout_flag) {
        while (1) {
            size_t lineNum;
            printf("Enter the line number (0 to exit): ");
            scanf("%lu", &lineNum);

            if (lineNum == 0) {
                break;
            }

            if (lineNum <= lineNumber) {
                printf("Line %lu: %.*s", lineNum, (int) lineTable[lineNum - 1].length,
                       fileContent + lineTable[lineNum - 1].offset);
            } else {
                printf("Line number out of range.\n");
            }
        }
    } else {
        printf("Timeout exceeded. Displaying entire file content:\n");
        printf("%.*s", (int) fileSize, fileContent);
    }

    free(lineTable);
    munmap(fileContent, fileSize);
    close(fd);

    return 0;
}
