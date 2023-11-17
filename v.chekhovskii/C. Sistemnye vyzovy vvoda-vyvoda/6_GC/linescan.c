#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>

int alarmTimeOut = 0;
void alarmFunction(int signum) {
    alarmTimeOut = 1;
    puts("\nInput time expired. Press any key to see entire file... ");
}

int main() {
    signal(SIGALRM, alarmFunction);
    printf("Enter file name you want to open, or '.' to exit: ");
    char file[256]; scanf("%s", file);
    if (file[0] == '.') {
        printf("\nGotcha. Exiting the program...\n");
        return 0;
    }
    FILE* f_file = open(file, O_RDONLY);
    if (f_file == -1) {
        perror("\nFailed to open file");
        return 1;
    }

    char fileData[1024];
    read(f_file, fileData, sizeof(fileData));
    // count lines and offsets in file
    int fileLines = 0, i = 0;
    int lineOffsets[1024]; lineOffsets[0] = 0; // line 1 always has offset =0
    while (fileData[i] != '\0') {
        if (fileData[i] == '\n') {
            lineOffsets[fileLines+1] = i+1;
            fileLines++;
        }
        i++;
    }

    printf("\nFile lines: %d\n", fileLines);
    printf("Line offsets:\n");
    for (int i = 0; i < fileLines; i++) {
        printf("- Line %d: %ld\n", i+1, lineOffsets[i]);
    }

    alarm(5);
    printf("\nEnter line you want to analyze: ");
    int line; scanf("%d", &line);

    if (!alarmTimeOut) {
        if (line <= 0 || line > fileLines) {
            perror("Error: line out of range!\n");
            return 2;
        }

        lseek(f_file, lineOffsets[line-1], SEEK_SET);   // move to a given line
        char lineData[1024];
        int bytesRead = read(f_file, lineData, (lineOffsets[line]-lineOffsets[line-1]-1));
        lineData[lineOffsets[line]-lineOffsets[line-1]-1] = '\0';
        printf("Line %d (%ld symbols): '%s'\n", line, bytesRead, lineData);
    }
    else {
        printf("Entire file contents:\n%s", fileData);
    }

    return 0;
}