#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct linkedList {
char line[128];
struct linkedList* next;
} linkedList;

int main() {
    linkedList lines;
    char lineBuffer[128];
    lines.next = NULL;

    while (1) {
        fgets(lineBuffer, sizeof(lineBuffer), stdin);
        if (lineBuffer[0] == '.') { break; }

        linkedList* currLine = &lines;
        while (currLine->next != NULL) {
            currLine = currLine->next;
        }
        linkedList* newLine = malloc(sizeof(linkedList));
        strcpy(newLine->line, lineBuffer);
        newLine->next = NULL;

        currLine->next = newLine;
    }

    linkedList* currLine = &lines;
    while (currLine->next != NULL) {
        currLine = currLine->next;
        printf("%s", currLine->line);
    }
}
