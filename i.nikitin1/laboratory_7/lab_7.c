#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char *argv[]) 
{

    assert(argc == 2);

    FILE* fp;
 
    if (!(fp = fopen(argv[1], "r"))) {
        perror("open error"); 
        exit(1); 
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
    printf("\nEnter line you want to analyze: ");
    int line; scanf("%d", &line);

    if (line <= 0 || line > fileLines) {
        perror("Error: line out of range!\n");
        return 2;
    }

    lseek(f_file, lineOffsets[line-1], SEEK_SET);   // move to a given line
    char lineData[1024];
    int bytesRead = read(f_file, lineData, (lineOffsets[line]-lineOffsets[line-1]-1));
    lineData[lineOffsets[line]-lineOffsets[line-1]-1] = '\0';
    printf("Line %d (%ld symbols): '%s'\n", line, bytesRead, lineData);

    return 0;
}